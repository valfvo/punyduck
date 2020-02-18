#!/usr/bin/env python3.8
import asyncio
import os
import re

#Longueur du nom qui n'est pas la longueur binaire

def encodeBuffer(message):
    if type(message) == bytes:
        print("\033[31mMessage : encodé\033[0m")
    else:
        print("\033[31mMessage : non encodé\033[0m")
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("\033[36mSize Buffer sent : ", int.from_bytes(sizeBuffer_send, "big"), "\033[0m")

    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    # print(f"\033[91mReceived size buffer : {sizeBuffer_received!r}\033[0m")

    return sizeBuffer_received

def extract_path(path):
    newPath = re.search('(../)*(.*)', path).group(2)

    return newPath

async def send_file(path, writer):
    writer.write(int(0).to_bytes(1, "big"))
    try:
        with open(path, "rb") as file:
            data = file.read()
            # nameFile = path.split('/')[-1].encode()
        lenghtpath = len(path.encode()).to_bytes(1, "big")
            # print("lenghtName = ", lenghtName)
        data = lenghtpath + path.encode() + data
    except IOError:
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert".encode()
    
    await send_message(writer, data)

async def receive_file(reader):
    # print("receive_file() begin")
    data = await receive_message(reader)
    # print("data read check")
    lenghtName = data[0]
    # print("lenghtname check : ", lenghtName)
    path = data[1:lenghtName+1]
    path = path.decode()
    # print("path = ", path)
    path = path.split('/')
    nameFile = path[-1]
    path2 = ""
    for i in path[0:-1]:
        path2 += i + '/'
    try:
        os.makedirs(path2)
    except:
        pass
    data = data[(lenghtName+1):]
    print("file received check ; path2 = ", path2, " nameFile = ", nameFile)
    path2 = extract_path(path2)
    with open(path2+nameFile, "wb") as file:
        file.write(data)

async def send_message(writer, message):
    sizeBuffer_send = encodeBuffer(message)
    writer.write(sizeBuffer_send)
    await writer.drain()
    # print(f'\033[36mMessage send : \033[93m{message}\033[0m')
    i = 0
    while len(message) > 0:
        # print(message[:1024], "\n\n")
        writer.write(message[:1024])
        message = message[1024:]
        i += 1
        if i == 10:
            await writer.drain()
            i = 0
        
    await writer.drain()

async def receive_message(reader):
    sizeBuffer_received = decodeBuffer(await reader.read(4))
    data = await reader.read(sizeBuffer_received)
    return data

async def send_dir(pathDir, writer):
    if os.path.isdir(pathDir):
        tree = os.listdir(pathDir)
        for fileordir in tree:
            if os.path.isdir(pathDir+'/'+fileordir):
                print("dir : ", pathDir+'/'+fileordir)
                await send_dir(pathDir+'/'+fileordir, writer)
            else:
                print("file : ", pathDir+'/'+fileordir)
                await send_file(pathDir+'/'+fileordir, writer)
                await writer.drain()
    else:
        await send_file(pathDir, writer)


async def main():
    reader, writer = await asyncio.open_connection('127.0.0.1', 50001)
    connexion = True

    while connexion:
        action = input("Voulez-vous envoyer un projet ou en télécharger un ? ")
        if action == "envoyer":
            path = input("Veuillez entrer le chemin du fichier à envoyer : ")
            await send_dir(path, writer)
        elif action == "telecharger":
            writer.write(int(1).to_bytes(1, "big"))
            await writer.drain()
            nameDir = input("Veuillez entrer le nom du fichier à télécharger : ")
            await send_message(writer, nameDir.encode())
            # print("send message check")
            await receive_file(reader)
            print(f"{nameDir} successfully downloaded")
        elif action == "quitter":
            writer.write(int(2).to_bytes(1, "big"))
            await writer.drain()
            connexion = False
        else:
            print('Veuillez entrer "envoyer", "telecharger" ou "quitter" svp.')

    print('Close the connection')
    writer.close()

asyncio.run(main())