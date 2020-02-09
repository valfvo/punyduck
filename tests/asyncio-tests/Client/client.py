#!/usr/bin/env python3.8
import asyncio
import os

def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")
    # print("\033[36mSize Buffer sent : ", int.from_bytes(sizeBuffer_send, "big"), "\033[0m")

    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    # print(f"\033[91mReceived size buffer : {sizeBuffer_received!r}\033[0m")

    return sizeBuffer_received

async def send_file(path, writer):
    try:
        with open(path, "rb") as file:
            data = file.read()
            # nameFile = path.split('/')[-1].encode()
            lenghtName = len(path).to_bytes(1, "big")
            # print("lenghtName = ", lenghtName)
            data = lenghtName + path.encode() + data
    except IOError:
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert"
    
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
    data = await reader.read(sizeBuffer_received)
    return data


async def main():
    reader, writer = await asyncio.open_connection('127.0.0.1', 8888)
    connexion = True

    while connexion:
        action = input("Voulez-vous envoyer un projet ou en télécharger un ? ")
        if action == "envoyer":
            writer.write(int(0).to_bytes(1, "big"))
            path = input("Veuillez entrer le chemin du fichier à envoyer : ")
            await send_file(path, writer)
        elif action == "telecharger":
            writer.write(int(1).to_bytes(1, "big"))
            nameFile = input("Veuillez entrer le nom du fichier à télécharger : ")
            await send_message(writer, nameFile)
            # print("send message check")
            await receive_file(reader)
            print(f"{nameFile} successfully downloaded")
        elif action == "quitter":
            writer.write(int(2).to_bytes(1, "big"))
            connexion = False
        else:
            print('Veuillez entrer "envoyer", "telecharger" ou "quitter" svp.')

    print('Close the connection')
    writer.close()

asyncio.run(main())