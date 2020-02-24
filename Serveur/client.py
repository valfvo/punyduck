#!/usr/bin/env python3.8
import asyncio
import os
import re

def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")

    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    # print("sizeBuffer_received = ", sizeBuffer_received)

    return sizeBuffer_received

def extract_path(path):
    newPath = re.search('(../)*(.*)', path).group(2)

    return newPath

async def send_file(path, writer):
    writer.write(int(0).to_bytes(1, "big"))
    try:
        with open(path, "rb") as file:
            await send_message(writer, path.encode())
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
    # print("data = ", data)
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
        # print("packet = ", packet)
        data[total_read:total_read+len(packet)] = packet
        total_read += len(packet)

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

asyncio.run(main())