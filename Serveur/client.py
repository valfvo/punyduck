#!/usr/bin/env python3.8
import asyncio
import os
import re
import sys

async def register(writer, reader):
    login = input("Entrez un login : ")
    await send_message(writer, login.encode())
    password = input("Choisissez un mot de passe : ")
    while len(password) < 8:
        password = input("Mot de passe trop court, choisissez un mot de passe : ")
    await send_message(writer, password.encode())

    loginValide = await reader.read(1)
    loginValide = int.from_bytes(loginValide, 'big')
    while loginValide == 0:
        print("Ce nom est déjà pris")
        login = input("Entrez un login : ")
        await send_message(writer, login.encode())
        loginValide = await reader.read(1)
        loginValide = int.from_bytes(loginValide, 'big')

    mail = input("Entrez une adrese mail : ")
    await send_message(writer, mail.encode())

    print("Vous vous êtes correctement inscrit")

async def login(writer, reader):
    login = input("Login : ")
    await send_message(writer, login.encode())
    password = input("Mot de passe : ")
    await send_message(writer, password.encode())
    response = await receive_message(reader)
    response = response.decode()
    print("response = ", response)

    return response

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


async def connexion():
    reader, writer = await asyncio.open_connection('127.0.0.1', 50001)
    user = await main(writer, reader)
    connexion = True
    while connexion:
        action = input("Voulez-vous envoyer un projet ou en télécharger un ? ")
        if action == "envoyer":
            writer.write(int(0).to_bytes(1, "big"))
            projectName = input("Veuillez donner un nom à votre projet : ")
            await send_message(writer, projectName.encode())
            response = await reader.read(1)
            # print("response = ", response)
            while int.from_bytes(response, 'big') == 0:
                print("Ce nom est déjà pris.")
                projectName = input("Veuillez donner un nom à votre projet : ")
                await send_message(writer, projectName.encode())
                response = await reader.read(1)

            path = input("Veuillez entrer le chemin du projet à envoyer : ")
            path = path.replace('\\', '/')
            pathServer = path.split('/')[-1]
            await send_message(writer, pathServer.encode())
            await send_message(writer, user.encode())
            await send_dir(path, writer, pathServer)
            writer.write(b'\x01')
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
        
    print('Close the connection')
    writer.close()

async def main(writer, reader):
    action =''
    while action != 'c' and action != 'i':
        action = input("Voulez-vous vous connecter ou vous inscrire ? (c/i) : ")
        if action == 'c':
            writer.write(int(0).to_bytes(1, "big"))
            user = await login(writer, reader)
            while user == "False":
                print("\033[31mLe nom d'utilisateur ou mot de passe est incorrect\033[0m")
                user = await login(writer, reader)
            return user
        elif action == 'i':
            writer.write(int(1).to_bytes(1, "big"))
            await register(writer, reader)
        else:
            print("Veuillez entre 'c' pour vous connecter ou 'i' pour vous inscrire")

asyncio.run(connexion())