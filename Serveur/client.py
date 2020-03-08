#!/usr/bin/env python3.8
import asyncio
import os
import re
import psycopg2
from argon2 import *
import sys

conn = psycopg2.connect("dbname=paulbunel user=paulbunel")
cur = conn.cursor()
ph = PasswordHasher()

def register():
    register = True
    while register:
        login = input("Entrez un login : ")
        cur.execute("SELECT login FROM UserInfo WHERE login = %s;", (login,))
        if cur.fetchone() != None:
            print("\033[31mCe nom est déjà pris\033[0m")
        else:
            register = False

    password = input("Choisissez un mot de passe : ")
    cur.execute("SELECT MAX(idLog) FROM UserInfo;")
    data = cur.fetchone()
    if data[0] == None:
        new_id = 1
    else:
        new_id = data[0]+1
    mail = input("Entrez une adrese mail : ")
    cur.execute("INSERT INTO UserInfo (idlog, login, password, email, admin) VALUES(%s, %s, %s, %s, FALSE)", (new_id, login, ph.hash(password), mail))
    conn.commit()
    print("Vous vous êtes correctement inscrit")

def login():
    login = input("Login : ")
    cur.execute("SELECT password FROM UserInfo WHERE login = %s", (login,))
    data = cur.fetchone()
    password = input("Mot de passe : ")
    try:
        ph.verify(data[0], password)
        connecting = False
        print("Bienvenue, ", login)
        return login
    except (TypeError, exceptions.VerifyMismatchError):
        return "False"

def delete_account(login):
    action = input("Êtes-vous sûr de vouloir supprimer votre compte ? (y/n)")
    if action == 'y':
        cur.execute("DELETE FROM UserInfo WHERE login = %s", (login,))
        conn.commit()

def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")

    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")

    return sizeBuffer_received

def extract_path(path):
    newPath = re.search('(../)*(.*)', path).group(2)

    return newPath

async def send_file(path, writer, oDir):
    writer.write(int(0).to_bytes(1, "big"))
    try:
        with open(path, "rb") as file:
            pathServer = ""
            test = False
            for i in path.split('/'):
                if i == oDir:
                    test = True
                if test:
                    pathServer += i+'/'
            pathServer = pathServer[:-1]
            await send_message(writer, pathServer.encode())
            filesize = os.path.getsize(path)
            writer.write(filesize.to_bytes(4, 'big'))
            loop = asyncio.get_running_loop()
            await loop.sendfile(writer.transport, file)
    except IOError:
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert".encode()
        await send_message(writer, data)

async def receive_file(reader):
    path = await receive_message(reader)
    print("\033[33mPath = ", path.decode(), "\033[0m")
    data = await receive_message(reader)
    path = path.decode()
    path = path.split('/')
    nameFile = path[-1]
    path2 = ""
    for i in path[0:-1]:
        path2 += i + '/'
    path2 = extract_path(path2)
    try:
        os.makedirs(path2)
    except:
        pass
    print("file received check ; path2 = ", path2, " nameFile = ", nameFile)
    with open(path2+nameFile, "wb") as file:
        file.write(data)
    print(path2+nameFile, " successfully downloaded")

async def send_message(writer, message):
    sizeBuffer_send = encodeBuffer(message)
    writer.write(sizeBuffer_send)
    await writer.drain()
    if len(message) > 1024:
        i = 0
        while i < len(message)/1024:
            writer.write(message[i*1024:(i+1)*1024])
            i += 1
            if i % 10 == 0:
                await writer.drain()
    else:
        writer.write(message)
        
    await writer.drain()

async def receive_message(reader):
    sizeBuffer_received = await reader.read(4)
    print("Size Buffer received = ", sizeBuffer_received)
    sizeBuffer_received = decodeBuffer(sizeBuffer_received)
    data = bytearray(sizeBuffer_received)
    packetsize = 1024
    total_read = 0
    while True:
        packetsize = min(sizeBuffer_received-total_read, packetsize)
        if packetsize <= 0:
            return data
        packet = await reader.read(packetsize)
        data[total_read:total_read+len(packet)] = packet
        total_read += len(packet)

    return data

async def send_dir(pathDir, writer, oDir):
    if os.path.isdir(pathDir):
        print(pathDir, "est un dossier")
        tree = os.listdir(pathDir)
        for fileordir in tree:
            if os.path.isdir(pathDir+'/'+fileordir):
                print("dir : ", pathDir+'/'+fileordir)
                await send_dir(pathDir+'/'+fileordir, writer, oDir)
            else:
                print("file : ", pathDir+'/'+fileordir)
                await send_file(pathDir+'/'+fileordir, writer, oDir)
                await writer.drain()
    elif os.path.isfile(pathDir):
        print(pathDir, "est un fichier")
        await send_file(pathDir, writer, oDir)
    else:
        print("Veuillez entrer un chemin valide")


def main():
    action =''
    while action != 'c' and action != 'd':
        action = input("Voulez-vous vous connecter ou vous inscrire ? (c/i) : ")
        if action == 'c':
            user = login()
            while user == "False":
                print("\033[31mLe nom d'utilisateur ou mot de passe est incorrect\033[0m")
                user = login()
            asyncio.run(connexion(user))
        elif action == 'i':
            register()
        else:
            print("Veuillez entre 'c' pour vous connecter ou 'i' pour vous inscrire")

async def connexion(user):
    reader, writer = await asyncio.open_connection('127.0.0.1', 50001)
    connexion = True
    while connexion:
        action = input("Voulez-vous envoyer un projet ou en télécharger un ? ")
        if action == "envoyer":
            projectName = input("Veuillez donner un nom à votre projet : ")
            cur.execute("SELECT nom FROM projet WHERE nom = %s", (projectName,))
            projectName_invalid = cur.fetchone() != None
            while projectName_invalid:
                print("Ce nom est déjà pris.")
                projectName = input("Veuillez donner un nom à votre projet : ")
                cur.execute("SELECT nom FROM projet WHERE nom = %s", (projectName,))
                projectName_invalid = cur.fetchone() != None

            path = input("Veuillez entrer le chemin du projet à envoyer : ")
            path = path.replace('\\', '/')
            cur.execute("SELECT MAX(idProjet) FROM Projet;")
            data = cur.fetchone()
            if data[0] == None:
                new_id = 1
            else:
                new_id = data[0]+1
            pathServer = path.split('/')[-1]
            cur.execute("SELECT idLog FROM UserInfo WHERE login = %s;", (user,))
            idlog = cur.fetchone()[0]
            await send_dir(path, writer, pathServer)
            cur.execute("INSERT INTO Projet VALUES(%s, %s, %s, FALSE, %s);", (new_id, pathServer, idlog, projectName))
        elif action == "telecharger":
            writer.write(int(1).to_bytes(1, "big"))
            await writer.drain()
            nameDir = input("Veuillez entrer le chemin du projet à télécharger : ")
            await send_message(writer, nameDir.encode())
            while True:
                print("Downloading project...")
                downloading = await reader.read(1)
                downloading = int.from_bytes(downloading, 'big')
                print("\033[33mDownloading = ", downloading, "\033[0m")
                if downloading:
                    await receive_file(reader)
                else:
                    break
            print(f"{nameDir} successfully downloaded")
        elif action == "quitter":
            writer.write(int(2).to_bytes(1, "big"))
            await writer.drain()
            connexion = False
        else:
            print('Veuillez entrer "envoyer", "telecharger" ou "quitter" svp.')
        
        conn.commit()

    print('Close the connection')
    writer.close()

main()
cur.close()
conn.close()