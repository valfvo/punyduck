#!/usr/bin/env python3.8
import asyncio
import os
import re
import sys
import gateway as gw

def encodeBuffer(message): #Fonction qui renvoie la taille d'un message en binaire
    sizeBuffer_send = len(message).to_bytes(4, "big")

    return sizeBuffer_send

def decodeBuffer(buffer): #Fonction qui décode un nombre encodé en binaire (appelée pour décoder la taille d'un message)
    sizeBuffer_received = int.from_bytes(buffer, "big")

    return sizeBuffer_received

async def send_file(path, writer, oDir): #Fonction envoyant un fichier au serveur. On passe en paramètre le chemin du fichier, le writer, et le dossier originel du projet
    writer.write(b'\x00') #On envoie un zéro pour indiquer au serveur qu'il va recevoir un fichier
    try:
        with open(path, "rb") as file:
            """On crée un string "pathServer" qui stockera le chemin à partir du dossier originel. Cela sera utile dans les cas où
            le dossier à transmettre se situera à plusieurs dossiers d'intervalle du client, on supprimera donc les dossiers intervalles
            du chemin."""
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
            writer.write(filesize.to_bytes(4, 'big')) #On envoie la taille du fichier
            loop = asyncio.get_running_loop()
            await loop.sendfile(writer.transport, file) #On envoie le fichier
    except IOError: #Si le fichier n'a pas pu être ouvert
        print("Erreur ! Le fichier", path, "n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert".encode()
        await send_message(writer, data)

async def receive_file(reader): #Fonction appelée pour recevoir un fichier envoyé par la fonction "send_file()" du serveur
    path = await receive_message(reader) #On récupère d'abord le chemin du fichier
    data = await receive_message(reader) #On récupère ensuite les données binaires du fichier
    path = path.decode()
    path = path.split('/')
    #Dans le chemin du fichier, on sépare le nom du fichier du reste du chemin
    nameFile = path[-1]
    path2 = 'Projects/'
    for i in path[0:-1]:
        path2 += i + '/'
    try:
        os.makedirs(path2) #On crée les dossiers parents si ceux-ci n'existent pas
    except:
        pass
    with open(path2+nameFile, "wb") as file:
        file.write(data) #On écrit les données binaire du fichier

async def send_message(writer, message): #Fonction pouvant envoyer un message au serveur
    sizeBuffer_send = encodeBuffer(message) #On récupère la taille du message à envoyer
    writer.write(sizeBuffer_send)

    if len(message) > 1024:
        i = 0
        while i < len(message)/1024: #On envoie le message kilo par kilo si celui-ci est suffisament grand
            writer.write(message[i*1024:(i+1)*1024])
            i += 1
            if i % 10 == 0:
                await writer.drain()
    else:
        writer.write(message)
        await writer.drain()
        
async def receive_message(reader): #Fonction pour récupérer un message envoyé par la fonction "send_message()" ou "loop.sendfile()" du serveur
    sizeBuffer_received = await reader.read(4) #On lit la taille du message à recevoir
    sizeBuffer_received = decodeBuffer(sizeBuffer_received)

    data = bytearray(sizeBuffer_received)
    packetsize = 1024
    total_read = 0
    while True: #On lit le message kilo par kilo, jusqu'à ce que celui-ci devienne plus petit qu'un kilo-octet
        packetsize = min(sizeBuffer_received-total_read, packetsize)
        if packetsize <= 0:
            return data
        packet = await reader.read(packetsize)
        data[total_read:total_read+len(packet)] = packet
        total_read += len(packet)

    return data

async def send_dir(pathDir, writer, oDir):
    """Fonction envoyant les fichiers d'un dossier de manière récursive. On lui passe en paramètre le chemin pour accéder au dossier originel,
    le writer, et le nom du dossier originel."""
    if os.path.isdir(pathDir):
        tree = os.listdir(pathDir)
        for fileordir in tree: #Pour chaque fichier/dossier du dossier originel :
            await send_dir(pathDir+'/'+fileordir, writer, oDir)
            await writer.drain()
    elif os.path.isfile(pathDir):
        await send_file(pathDir, writer, oDir)


async def logicalResponse(writer, reader, event, size_to_read):
    result = await reader.read(size_to_read)
    if result == int(0).to_bytes(size_to_read, 'big'):
        gw.transmit_response(event+b'\0'+int(0).to_bytes(1, 'big'))
    else:
        gw.transmit_response(event+b'\0'+int(1).to_bytes(1, 'big'))
    return result

async def upProject(writer, path, oDir, idLog):
    #On envoie ensuite le chemin du projet ainsi que le login de l'utilisateur au serveur
    await send_dir(path, writer, oDir) #Pour finir, on appelle la fonction send_dir() pour envoyer le projet au serveur
    writer.write(b'\x01') #On envoie un 1 pour indiquer qu'il n'y a plus rien à télécharger
    
    writer.write(idLog)
    print("PROJET ENVOYE")

async def getProject(writer, reader):
    downloading = await reader.read(1) # Variable pour savoir s'il reste des fichiers à recevoir
    downloading = int.from_bytes(downloading, "big")
    ddl = False
    while downloading == 0:
        await receive_file(reader)
        downloading = await reader.read(1)
        downloading = int.from_bytes(downloading, "big")
        ddl = True
    print("Project downloaded")
    return ddl


async def main():
    """Fonction principale, appelée au début du programme. Après avoir établi la connexion avec le serveur, elle appelle la fonction connexion
    qui gère la connexion à son compte. Ensuite, on demande à l'utilisateur s'il souhaite envoyer ou recevoir un projet."""
    reader, writer = await asyncio.open_connection('88.122.55.128', 18545)
    print('connection established')
    connected = True
    idLog = b'\x00\00'
    while connected: #Tant qu'on est connecté au serveur, on demande si l'utilisateur veut envoyer ou recevoir un projet
        action = gw.poll_request()
        if action:
            task = asyncio.create_task(send_message(writer, action))
            await task
            if action == b'\x00': #Se déconnecter
                connected = False

            elif action.tobytes()[0] == 1: #DataQuery
                infos= bytes(await receive_message(reader))
                gw.transmit_response(infos)

            elif action.tobytes()[0] == 2: #Login
                idLog = await logicalResponse(writer, reader, "login".encode(), 2)

            elif action.tobytes()[0] == 3: #Register
                await logicalResponse(writer, reader, "register".encode(), 1)
            
            elif action.tobytes()[0] == 4:
                checkName = await logicalResponse(writer, reader, "upProject".encode(), 1)
                if int.from_bytes(checkName, 'big'):
                    infos = ((action.tobytes())[1:]).decode()
                    matchInfos = infos.split('|')
                    path = matchInfos[0]
                    path = path.replace('\\', '/')
                    oDir = path.split('/')[-1]
                    task = asyncio.create_task(upProject(writer, path, oDir, idLog))
                    await task
                    pathImage = matchInfos[4]
                    if pathImage != "":
                        pathImage = pathImage.replace('\\', '/')
                        oDir = pathImage.split('/')[-1]
                        task2 = asyncio.create_task(send_file(pathImage, writer, oDir))
                        await task2
                    gw.transmit_response("projectUploaded".encode()+b'\0'+int(1).to_bytes(1, 'big'))

            elif action.tobytes()[0] == 5:
                #On vérifie à chaque tour si il y a encore un fichier à télécharger ou non. Si oui, on appelle receive_file()
                ddl = await getProject(writer, reader)
                if ddl:
                    gw.transmit_response("projectDownloaded".encode()+b'\0'+int(1).to_bytes(1, 'big'))
                else:
                    gw.transmit_response("projectDownloaded".encode()+b'\0'+int(0).to_bytes(1, 'big'))

    print('Close the connection')
    writer.close()

asyncio.run(main())
