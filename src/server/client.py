#!/usr/bin/env python3.8
import asyncio
import os
import re
import sys
import litequarks as lq
from argon2 import *

ph = PasswordHasher()
 
async def register(writer, reader): #Fonction appelée pour inscrire un nouvel utilisateur
    #On envoie un login et un mot de passe au serveur
    login = input("Entrez un login : ")
    password = input("Choisissez un mot de passe : ")
    while len(password) < 8:
        password = input("Mot de passe trop court, choisissez un mot de passe : ")
    
    mail = input("Entrez une adrese mail : ")

    query = "INSERT INTO UserInfo (login, password, email, admin, idImage, idBio) VALUES('%s', '%s', '%s', FALSE, 1, 1)" % (login, ph.hash(password), mail)
    print(type(query), " Query = ", query)
    await send_message(writer, query.encode())

    #Le serveur nous dit si le login est disponible, ou s'il est déjà utilisé par un autre compte. Dans ce cas, on en entre un nouveau
    loginValide = await reader.read(1)
    print("loginvalide = ", loginValide)
    while loginValide == b'\x00': #Tant que le login n'est pas valide, on envoie un nouveau login au serveur pour le tester
        print("Ce nom est déjà pris")
        login = input("Entrez un login : ")
        password = input("Choisissez un mot de passe : ")
        while len(password) < 8:
            password = input("Mot de passe trop court, choisissez un mot de passe : ")
        
        mail = input("Entrez une adrese mail : ")

        query = "INSERT INTO UserInfo (login, password, email, admin, idImage, idBio) VALUES('%s', '%s', '%s', FALSE, 1, 1)" % (login, ph.hash(password), mail)
        await send_message(writer, query.encode())
        loginValide = await reader.read(1)

    print("Vous vous êtes correctement inscrit")
    lq.transmit_response(b'\x01')

async def login(writer, reader): #Fonction appelée quand un utilisateur souhaite se connecter
    #On envoie un login et un mot de passe au serveur
    login = input("Login : ")
    await send_message(writer, login.encode())
    password = input("Mot de passe : ")
    await send_message(writer, password.encode())

    #Si les identifiants sont bons, le serveur renvoie le login, sinon il renvoie "False".
    response = await receive_message(reader)
    response = response.decode()
    print("response = ", response)
    lq.transmit_response(login.encode())
    while response == "False": #Tant que les identifiants sont incorrects, on en envoie de nouveaux au serveur pour les tester
        print("\033[31mLe nom d'utilisateur ou mot de passe est incorrect\033[0m")
        login = input("Login : ")
        await send_message(writer, login.encode())
        password = input("Mot de passe : ")
        await send_message(writer, password.encode())

    return response #On retourne le login

def encodeBuffer(message): #Fonction qui renvoie la taille d'un message en binaire
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("sizeBuffer_send : ", len(message))

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
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert".encode()
        await send_message(writer, data)

async def receive_file(reader): #Fonction appelée pour recevoir un fichier envoyé par la fonction "send_file()" du serveur
    path = await receive_message(reader) #On récupère d'abord le chemin du fichier
    print("\033[33mPath = ", path.decode(), "\033[0m")
    data = await receive_message(reader) #On récupère ensuite les données binaires du fichier
    path = path.decode()
    path = path.split('/')
    #Dans le chemin du fichier, on sépare le nom du fichier du reste du chemin
    nameFile = path[-1]
    path2 = ""
    for i in path[0:-1]:
        path2 += i + '/'

    try:
        os.makedirs(path2) #On crée les dossiers parents si ceux-ci n'existent pas
    except:
        pass
    print("file received check ; path2 = ", path2, " nameFile = ", nameFile)
    with open(path2+nameFile, "wb") as file:
        file.write(data) #On écrit les données binaire du fichier
    print(path2+nameFile, " successfully downloaded")

async def send_message(writer, message): #Fonction pouvant envoyer un message au serveur
    sizeBuffer_send = encodeBuffer(message) #On récupère la taille du message à envoyer
    writer.write(sizeBuffer_send)

    if len(message) > 1024:
        print("test")
        i = 0
        while i < len(message)/1024: #On envoie le message kilo par kilo si celui-ci est suffisament grand
            writer.write(message[i*1024:(i+1)*1024])
            i += 1
            if i % 10 == 0:
                await writer.drain()
    else:
        print("message to send : ", message, ", type : ", type(message))
        writer.write(message)
        await writer.drain()
        print("message envoyé")
        
async def receive_message(reader): #Fonction pour récupérer un message envoyé par la fonction "send_message()" ou "loop.sendfile()" du serveur
    sizeBuffer_received = await reader.read(4) #On lit la taille du message à recevoir
    print("Size Buffer received = ", sizeBuffer_received)
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
        print(pathDir, "est un dossier")
        tree = os.listdir(pathDir)
        for fileordir in tree: #Pour chaque fichier/dossier du dossier originel :
            if os.path.isdir(pathDir+'/'+fileordir): #Si c'est un dossier, on appelle récursivement la fonction send_dir() sur ce dossier
                print("dir : ", pathDir+'/'+fileordir)
                await send_dir(pathDir+'/'+fileordir, writer, oDir)
            else: #Sinon, si c'est un fichier, on appelle la fonction send_file() sur ce fichier
                print("file : ", pathDir+'/'+fileordir)
                await send_file(pathDir+'/'+fileordir, writer, oDir)
                await writer.drain()
    elif os.path.isfile(pathDir):
        print(pathDir, "est un fichier")
        await send_file(pathDir, writer, oDir)
    else:
        print("Veuillez entrer un chemin valide")

async def get_info_ProjetValides(idProjet, writer, reader):
    writer.write(idProjet.to_bytes(1, 'big'))
    valide = int.from_bytes(await reader.read(1), 'big')
    if valide:
        idLog = int.from_bytes(await reader.read(1), 'big')
        nom = (await receive_message(reader)).decode()
        tags = (await receive_message(reader)).decode()
        idBio = int.from_bytes(await reader.read(1), 'big')
        idImage = int.from_bytes(await reader.read(1), 'big')
        print(idLog, nom, tags, idBio, idImage, sep=" ")


async def main():
    """Fonction principale, appelée au début du programme. Après avoir établi la connexion avec le serveur, elle appelle la fonction connexion
    qui gère la connexion à son compte. Ensuite, on demande à l'utilisateur s'il souhaite envoyer ou recevoir un projet."""
    reader, writer = await asyncio.open_connection('127.0.0.1', 50002)
    connected = True
    while connected: #Tant qu'on est connecté au serveur, on demande si l'utilisateur veut envoyer ou recevoir un projet
        action = lq.poll_request()
        if action:
            task = asyncio.create_task(send_message(writer, action))
            print("action : ", action, " action.tobytes() : ", action.tobytes())
            await task
            if action == b'0':
                connected = False
            elif action[0] == 1:
                
            elif action.tobytes()[0] == 49:
                result = await reader.read(1)
                print("result : ", result)
                while result == b'\x00':
                    print("111111111111111111")
                    lq.transmit_response(b'\x30')
                    print("222222222222222222")
                    action = None
                    while True:
                        action = lq.poll_request()
                        if action != None:
                            print("action : ", action.tobytes())
                            break
                    task = asyncio.create_task(send_message(writer, action))
                    await task
                    result = await reader.read(1)
                lq.transmit_response(b'')
                

            # await asyncio.sleep(1000)

            # await send_message(writer, action)
            # action = action.decode()
            # print("action : ", action)
            # if action == "login":
            #     writer.write(b'\x00') #On envoie 0 au serveur pour qu'il sache que le client va se connecter
            #     user = await login(writer, reader)

            # elif action == "register":
            #     writer.write(b'\x01') #On envoie 1 au serveur pour qu'il sache que le client va s'inscrire
            #     await register(writer, reader)
                
            # elif action == "up_project":
            #     writer.write(b'\x02') #On envoie un zéro au serveur pour indiquer qu'il va recevoir les informations d'un projet

            #     #On envoie au serveur le nom du projet choisit par l'utilisateur. Il nous renvoie une réponse indiquant la disponibilité du nom
            #     projectName = input("Veuillez donner un nom à votre projet : ")
            #     await send_message(writer, projectName.encode())
            #     response = await reader.read(1)
            #     while int.from_bytes(response, 'big') == 0: #Si le nom n'est pas disponible, on renvoie un nouveau nom jusqu'à ce qu'il soit disponible
            #         print("Ce nom est déjà pris.")
            #         projectName = input("Veuillez donner un nom à votre projet : ")
            #         await send_message(writer, projectName.encode())
            #         response = await reader.read(1)

            #     #On envoie ensuite le chemin du projet ainsi que le login de l'utilisateur au serveur
            #     path = input("Veuillez entrer le chemin du projet à envoyer : ")
            #     path = path.replace('\\', '/')
            #     pathServer = path.split('/')[-1]
            #     await send_message(writer, pathServer.encode())
            #     await send_message(writer, user.encode())
            #     await send_dir(path, writer, pathServer) #Pour finir, on appelle la fonction send_dir() pour envoyer le projet au serveur
            #     writer.write(b'\x01') #On envoie un 1 pour indiquer qu'il n'y a plus rien à télécharger

            # elif action == "dl_project":
            #     writer.write(b'\x03') #On envoie un 1 au serveur pour lui signifier qu'il doit envoyer un projet au client
            #     idProjet = input("Veuillez entrer l'id du projet à télécharger : ")
            #     idProjet = int(idProjet)
            #     writer.write(idProjet.to_bytes(1, 'big')) #On envoie au serveur le chemin du projet à télécharger
            #     while True:
            #         #On vérifie à chaque tour si il y a encore un fichier à télécharger ou non. Si oui, on appelle receive_file()
            #         print("Downloading project...")
            #         downloading = await reader.read(1)
            #         downloading = int.from_bytes(downloading, 'big')
            #         print("\033[33mDownloading = ", downloading, "\033[0m")
            #         if downloading:
            #             await receive_file(reader)
            #         else:
            #             break
            #     print("Project successfully downloaded")

            # elif action == "infos":
            #     writer.write(b'\x04')
            #     idProjet = input("Entrez l'id du projet dont vous voulez les infos : ")
            #     await get_info_ProjetValides(int(idProjet), writer, reader)


    print('Close the connection')
    writer.close()

# async def connexion(writer, reader): #Fonction qui gère la connection au compte de l'utilisateur
#     action =''
#         action = input("Voulez-vous vous connecter ou vous inscrire ? (c/i) : ")
#         if action == 'c': #Si l'utilisateur souhaite se connecter, on appelle la fonction login()
#             writer.write(int(0).to_bytes(1, "big")) #On envoie 0 au serveur pour qu'il sache que le client va se connecter
#             user = await login(writer, reader)
#             return user
#         elif action == 'i': #S'il souhaite s'inscrire, on appelle la fonction register
#             writer.write(int(1).to_bytes(1, "big")) #On envoie 1 au serveur pour qu'il sache que le client va s'inscrire
#             await register(writer, reader)
#         else:
#             print("Veuillez entre 'c' pour vous connecter ou 'i' pour vous inscrire")

asyncio.run(main())