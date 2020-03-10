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

def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("Message = ", message, "sizeBuffer_send = ", sizeBuffer_send)

    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    
    return sizeBuffer_received

async def send_file(path, writer, oDir):
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
    data = await receive_message(reader)
    path = path.decode()
    path = path.split('/')
    nameFile = path[-1]
    path2 = ""
    for i in path[0:-1]:
        path2 += i + '/'
    try:
        os.makedirs(path2)
    except:
        pass
    print("file received check ; path2 = ", path2, " nameFile = ", nameFile)
    with open(path2+nameFile, "wb") as file:
        file.write(data)

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
    sizeBuffer_received = decodeBuffer(await reader.read(4))
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
                writer.write(int(1).to_bytes(1, "big"))
                await send_file(pathDir+'/'+fileordir, writer, oDir)
                await writer.drain()
    elif os.path.isfile(pathDir):
        writer.write(int(1).to_bytes(1, "big"))
        print(pathDir, "est un fichier")
        await send_file(pathDir, writer, oDir)
    else:
        print("Veuillez entrer un chemin valide")

async def log(action, writer, reader):
    action = int.from_bytes(action, 'big')
    login = await receive_message(reader)
    login = login.decode()
    password = await receive_message(reader)
    password = password.decode()
    if action:
        cur.execute("SELECT login FROM UserInfo WHERE login = %s;", (login,))
        if cur.fetchone() != None: #Si le login est déjà pris
            writer.write(int(0).to_bytes(1, "big"))
            loginValide = False
        else: #Sinon, s'il est disponible
            writer.write(int(1).to_bytes(1, "big"))
            loginValide = True
        while loginValide == False:
            login = await receive_message(reader)
            login = login.decode()
            cur.execute("SELECT login FROM UserInfo WHERE login = %s;", (login,))
            if cur.fetchone() != None: #Si le login est déjà pris
                writer.write(int(0).to_bytes(1, "big"))
                loginValide = False
            else: #Sinon, s'il est disponible
                writer.write(int(1).to_bytes(1, "big"))
                loginValide = True
        
        cur.execute("SELECT MAX(idLog) FROM UserInfo;")
        data = cur.fetchone()
        if data[0] == None:
            new_id = 1
        else:
            new_id = data[0]+1
        mail = await receive_message(reader)
        mail = mail.decode()
        cur.execute("INSERT INTO UserInfo (idlog, login, password, email, admin) VALUES(%s, %s, %s, %s, FALSE)", (new_id, login, ph.hash(password), mail))
        conn.commit()
    else:
        incorrectLog = True
        while incorrectLog:
            cur.execute("SELECT password FROM UserInfo WHERE login = %s", (login,))
            data = cur.fetchone()
            try:
                ph.verify(data[0], password)
                connecting = False
                print("Bienvenue, ", login)
                await send_message(writer, login.encode())
                incorrectLog = False
            except (TypeError, exceptions.VerifyMismatchError):
                print("caca")
                await send_message(writer, "False".encode())
                login = await receive_message(reader)
                login = login.decode()
                password = await receive_message(reader)
                password = password.decode()        

def delete_account(login):
    action = input("Êtes-vous sûr de vouloir supprimer votre compte ? (y/n)")
    if action == 'y':
        cur.execute("DELETE FROM UserInfo WHERE login = %s", (login,))
        conn.commit()

async def handle_echo(reader, writer):
    action = await reader.read(1)
    await log(action, writer, reader)
    connexion = True
    while connexion:
        action = await reader.read(1)
        action = int.from_bytes(action, "big")
        print("action = ", action)
        if action == 0:
            projectName = await receive_message(reader)
            projectName = projectName.decode()
            cur.execute("SELECT nom FROM projet WHERE nom = %s", (projectName,))
            projectName_invalid = cur.fetchone() != None
            while projectName_invalid:
                writer.write(int(0).to_bytes(1, "big"))
                print("Ce nom est déjà pris.")
                projectName = await receive_message(reader)
                projectName = projectName.decode()
                cur.execute("SELECT nom FROM projet WHERE nom = %s", (projectName,))
                projectName_invalid = cur.fetchone() != None
                # print("projectName_invalid = ", projectName_invalid)
            writer.write(int(1).to_bytes(1, "big"))

            cur.execute("SELECT MAX(idProjet) FROM Projet;")
            data = cur.fetchone()
            if data[0] == None:
                new_id = 1
            else:
                new_id = data[0]+1
                
            pathServer = await receive_message(reader)
            pathServer = pathServer.decode()
            user = await receive_message(reader)
            user = user.decode()

            cur.execute("SELECT idLog FROM UserInfo WHERE login = %s;", (user,))
            idlog = cur.fetchone()[0]

            cur.execute("INSERT INTO Projet VALUES(%s, %s, %s, FALSE, %s);", (new_id, pathServer, idlog, projectName))

            print("Réception d'un fichier client")
            downloading = await reader.read(1)
            downloading = int.from_bytes(downloading, "big")
            while downloading == 0:
                await receive_file(reader)
                downloading = await reader.read(1)
                downloading = int.from_bytes(downloading, "big")
                print("Downloading = ", downloading)
            conn.commit()
        elif action == 1:
            print("Envoi d'un fichier au client")
            path = await receive_message(reader)
            path = path.decode()
            print(f"\033[36mClient ask for {path}\033[0m")
            oDir = path.split('/')[-1]
            await send_dir(path, writer, oDir)
            writer.write(b'\x00')
            print('dossier envoye')
        elif action == 2:
            connexion = False
        else:
            print("Error action.")

        await writer.drain()
    print("Close the connection")
    writer.close()


async def main():
    server = await asyncio.start_server(
        handle_echo, '127.0.0.1', 50001)

    addr = server.sockets[0].getsockname()
    print(f'Serving on {addr}')

    async with server:
        await server.serve_forever()

asyncio.run(main())