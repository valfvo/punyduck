#!/usr/bin/env python3.8
import asyncio
import os
import re

def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")
    # print("\033[91mSize Buffer sent : ", int.from_bytes(sizeBuffer_send, "big"), "\033[0m")
    
    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    print(f"\033[36mReceived size buffer : {sizeBuffer_received!r}\033[0m")
    
    return sizeBuffer_received

def extract_path(path):
    newPath = re.search('(../)*(.*)', path).group(2)
    
    return newPath

async def send_file(path, writer):
    try:
        with open(path, "rb") as file:
            data = file.read()
            # nameFile = path.split('/')[-1].encode()
            lenghtPath = len(path).to_bytes(1, "big")
            data = lenghtPath + path + data
    except IOError:
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert"
    
    await send_message(writer, data)

async def receive_file(reader):
    data = await receive_message(reader)
    # print("data = ", data)
    lenghtPath = data[0]
    path = data[1:lenghtPath+1]
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
    data = data[(lenghtPath+1):]
    # print(data)
    print("file received check ; path2 = ", path2, " nameFile = ", nameFile)
    with open(path2+nameFile, "wb") as file:
        file.write(data)

async def send_message(writer, message):
    sizeBuffer_send = encodeBuffer(message)
    writer.write(sizeBuffer_send)
    # print(f'\033[36mMessage send : \033[93m{message}\033[0m')
    try:
        writer.write(message.encode())
    except AttributeError:
        writer.write(message)

async def receive_message(reader):
    sizeBuffer_received = decodeBuffer(await reader.read(4))
    data = bytearray()
    chuncksize = 1024
    while sizeBuffer_received > 0:
        if sizeBuffer_received < chuncksize:
            chuncksize = sizeBuffer_received
        packet = await reader.read(chuncksize)
        data += packet
        sizeBuffer_received -= len(packet)
        # if sizeBuffer_received >= 1024:
            # data += await reader.read(1024)
        # else:
            # data += await reader.read(sizeBuffer_received)
        # sizeBuffer_received -= 1024 #- len(data)

    return data

async def send_dir(pathDir):
    tree = os.listdir(pathDir)
    for file in tree:
        if os.path.isdir(file):
            await send_dir(file)
        else:
            await send_file(file)


async def handle_echo(reader, writer):
    connexion = True
    while connexion:
        action = await reader.read(1)
        action = int.from_bytes(action, "big")
        print("action = ", action)
        if action == 0:
            print("Réception d'un fichier client")
            await receive_file(reader)
        elif action == 1:
            print("Envoi d'un fichier au client")
            nameFile = await receive_message(reader)
            print(f"\033[36mClient ask for {extract_path(nameFile.decode())!r}")
            await send_file(nameFile, writer)
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