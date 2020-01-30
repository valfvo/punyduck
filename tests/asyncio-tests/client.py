#!/usr/bin/env python3.8
####### CACA #######
import asyncio
import sys

def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("\033[36mSize Buffer sent : ", int.from_bytes(sizeBuffer_send, "big"), "\033[0m")

    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    print(f"\033[91mReceived size buffer : {sizeBuffer_received!r}\033[0m")

    return sizeBuffer_received

async def encode_file(path):
    try:
        with open(path) as file:
            data = file.read()
    except IOError:
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert"
    
    return data

async def download_file(data):
    with open("FileTest.py", "w") as file:
        file.write(data)

async def send_toServer(writer, message):
    sizeBuffer_send = encodeBuffer(message)
    writer.write(sizeBuffer_send)
    # print(f'\033[36mMessage send : \033[93m{message}\033[0m')
    writer.write(message.encode())

async def receive_fromServer(reader):
    sizeBuffer_received = decodeBuffer(await reader.read(4))
    data = await reader.read(sizeBuffer_received)

    return data.decode()


async def main():
    reader, writer = await asyncio.open_connection('192.168.0.16', 8888)
    connexion = True

    while connexion:
        action = input("Voulez-vous envoyer un projet ou en télécharger un ? ")
        if action == "envoyer":
            writer.write(int(0).to_bytes(1, "big"))
            path = input("Veuillez entrer le chemin du fichier à envoyer : ")
            await send_toServer(writer, await encode_file(path))
        elif action == "telecharger":
            writer.write(int(1).to_bytes(1, "big"))
            nameFile = input("Veuillez entrer le nom du fichier à télécharger : ")
            await send_toServer(writer, nameFile)
            message = await receive_fromServer(reader)
            if "Erreur" in message:
                print(message)
            else:
                await download_file(message)
                print(f"{nameFile} successfully downloaded")
        elif action == "quitter":
            writer.write(int(2).to_bytes(1, "big"))
            connexion = False
        else:
            print('Veuillez entrer "envoyer", "telecharger" ou "quitter" svp.')

    print('Close the connection')
    writer.close()

asyncio.run(main())