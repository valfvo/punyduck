#!/usr/bin/env python3.8
import asyncio
import os

def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("\033[91mSize Buffer sent : ", int.from_bytes(sizeBuffer_send, "big"), "\033[0m")
    
    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    print(f"\033[36mReceived size buffer : {sizeBuffer_received!r}\033[0m")
    
    return sizeBuffer_received

async def send_file(path, writer):
    try:
        with open(path, "rb") as file:
            data = file.read()
            # nameFile = path.split('/')[-1].encode()
            lenghtName = len(path).to_bytes(1, "big")
            data = lenghtName + path + data
    except IOError:
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert"
    
    await send_message(writer, data)

async def receive_file(reader):
    data = await receive_message(reader)
    lenghtName = data[0]
    path = data[1:lenghtName+1]
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
    data = data[(lenghtName+1):]
    print("file received check ; path2 = ", path2, " nameFile = ", nameFile)
    with open(nameFile, "wb") as file:
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
    data = await reader.read(sizeBuffer_received)

    return data

async def handle_echo(reader, writer):
    connexion = True
    while connexion:
        action = await reader.read(1)
        action = int.from_bytes(action, "big")
        print("action = ", action)
        if action == 0:
            await receive_file(reader)
        elif action == 1:
            nameFile = await receive_message(reader)
            print(f"\033[36mClient ask for {nameFile.decode()!r}")
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
        handle_echo, '127.0.0.1', 8888)

    addr = server.sockets[0].getsockname()
    print(f'Serving on {addr}')

    async with server:
        await server.serve_forever()

asyncio.run(main())