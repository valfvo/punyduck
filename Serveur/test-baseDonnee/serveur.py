#!/usr/bin/env python3.8
import asyncio
import os
import re

def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("Message = ", message, "sizeBuffer_send = ", sizeBuffer_send)

    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    
    return sizeBuffer_received

def extract_path(path):
    newPath = re.search('(../)*(.*)', path).group(2)
    
    return newPath

async def send_file(path, writer):
    try:
        with open(path, "rb") as file:
            await send_message(writer, path.encode())
            filesize = os.path.getsize(path)
            print("filesize = ", filesize)
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
    path2 = extract_path(path2)
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

async def send_dir(pathDir, writer):
    if os.path.isdir(pathDir):
        tree = os.listdir(pathDir)
        for fileordir in tree:
            if os.path.isdir(pathDir+'/'+fileordir):
                print("dir : ", pathDir+'/'+fileordir)
                await send_dir(pathDir+'/'+fileordir, writer)
            else:
                print("file : ", pathDir+'/'+fileordir)
                writer.write(int(1).to_bytes(1, "big"))
                await send_file(pathDir+'/'+fileordir, writer)
    else:
        writer.write(int(1).to_bytes(1, "big"))
        await send_file(pathDir, writer)


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
            path = await receive_message(reader)
            print(f"\033[36mClient ask for {extract_path(path.decode())!r}\033[0m")
            await send_dir(path.decode(), writer)
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