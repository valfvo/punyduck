#!/usr/bin/env python3.8
import asyncio
import os
import re
import psycopg2
from argon2 import *
import sys

#Création du connecteur à la base de donnée, de son curseur et du hasher de mot de passe
conn = psycopg2.connect("dbname=paulbunel user=paulbunel")
cur = conn.cursor()
ph = PasswordHasher()


def encodeBuffer(message): #Fonction qui renvoie la taille d'un message en binaire
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("Message = ", message, "sizeBuffer_send = ", sizeBuffer_send)

    return sizeBuffer_send

def decodeBuffer(buffer): #Fonction qui décode un nombre encodé en binaire (appelée pour décoder la taille d'un message)
    sizeBuffer_received = int.from_bytes(buffer, "big")
    
    return sizeBuffer_received

async def send_file(path, writer, oDir): #Fonction envoyant un fichier au serveur. On passe en paramètre le chemin du fichier, le writer, et le dossier originel du projet
    writer.write(b'\x01') #On écrit un pour dire au client qu'il va recevoir un fichier
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

async def send_message(writer, message): #Fonction pouvant envoyer un message au serveur
    sizeBuffer_send = encodeBuffer(message) #On récupère la taille du message à envoyer
    writer.write(sizeBuffer_send)
    await writer.drain()
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
    sizeBuffer_received = decodeBuffer(await reader.read(4)) #On lit la taille du message à recevoir
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

async def send_info_ProjetValides(writer, reader):
    idProjet = await reader.read(1)
    idProjet = int.from_bytes(idProjet, 'big')
    cur.execute("SELECT valide FROM Projet WHERE idProjet = %s;", (idProjet,))
    valide = cur.fetchone()[0]
    print(valide)
    if valide:
        writer.write(b'\x01')
        cur.execute("SELECT idLog, nom, tags, idBio, idImage FROM Projet WHERE idProjet = %s;", (idProjet,))
        data = cur.fetchone()
        print(data)
        writer.write(data[0].to_bytes(1, 'big'))
        await send_message(writer, data[1].encode())
        await send_message(writer, data[2].encode())
        writer.write(data[3].to_bytes(1, 'big'))
        writer.write(data[4].to_bytes(1, 'big'))
    else:
        writer.write(b'\x00')

async def log(action, writer, reader):
    """Fonction appelée en premier lors de la connexion avec un client. Celle-ci gère la connexion du client à son compte,
    en effectuant les requêtes SQL adéquates. Elle prend en paramètre action, qui est un booléen (0/1) envoyé par le client
    pour indiquer s'il souhaite se connecter ou s'inscrire, ainsi que le writer et le reader."""
    action = int.from_bytes(action, 'big')
    #On récupère le login et le mot de passe saisis par le client
    login = await receive_message(reader)
    login = login.decode()
    password = await receive_message(reader)
    password = password.decode()
    if action: #Si le client souhaite s'inscrire
        #On va en premier vérifier la disponibilité du login
        cur.execute("SELECT login FROM UserInfo WHERE login = %s;", (login,)) #On recherche un login du même nom dans la base de données
        if cur.fetchone() != None: #Si le login est déjà pris
            writer.write(int(0).to_bytes(1, "big")) #On envoie 0 au client pour lui dire que le nom est déjà pris
            loginValide = False
        else: #Sinon, s'il est disponible
            writer.write(int(1).to_bytes(1, "big")) #On envoie 1 au client pour lui dire que le nom est libre
            loginValide = True
        while loginValide == False: #Si le login n'est pas disponible, on récupère le nouveau login jusqu'à ce que ce soit bon
            login = await receive_message(reader)
            login = login.decode()
            cur.execute("SELECT login FROM UserInfo WHERE login = %s;", (login,))
            if cur.fetchone() != None: #Si le login est déjà pris
                writer.write(int(0).to_bytes(1, "big"))
                loginValide = False
            else: #Sinon, s'il est disponible
                writer.write(int(1).to_bytes(1, "big"))
                loginValide = True
        
        #Ensuite, on incrémente de 1 le dernier id enregistré dans la base pour créer un nouvel id
        cur.execute("SELECT MAX(idLog) FROM UserInfo;")
        data = cur.fetchone()
        if data[0] == None:
            new_id = 1
        else:
            new_id = data[0]+1

        mail = await receive_message(reader)
        mail = mail.decode()

        #Enfin, on insère dans la table UserInfo une nouvelle ligne, avec les informations du nouveau compte
        cur.execute("INSERT INTO UserInfo (idlog, login, password, email, admin) VALUES(%s, %s, %s, %s, FALSE)", (new_id, login, ph.hash(password), mail))
        conn.commit()

    else: #Si le client souhaite s'inscrire
        incorrectLog = True
        while incorrectLog: #Tant que les identifiants sont incorrects
            cur.execute("SELECT password FROM UserInfo WHERE login = %s", (login,)) #On récupère le mot de passe correspondant au login
            data = cur.fetchone()
            try:
                ph.verify(data[0], password) #On vérifie que le mot de passe saisie corresponde à celui récupéré (qui est hashé)
                connecting = False
                print("Bienvenue, ", login)
                await send_message(writer, login.encode()) #On envoie au client le login pour lui indiquer qu'il s'est connecté
                incorrectLog = False
            except (TypeError, exceptions.VerifyMismatchError): #Si les identifiants sont incorrects, on en reçoit de nouveaux
                await send_message(writer, "False".encode()) #On envoie "False" pour signifier au client que les identifiants sont incorrects
                login = await receive_message(reader)
                login = login.decode()
                password = await receive_message(reader)
                password = password.decode()        

def delete_account(login): #Fonction appelée pour supprimer un compte
    action = input("Êtes-vous sûr de vouloir supprimer votre compte ? (y/n)")
    if action == 'y':
        cur.execute("DELETE FROM UserInfo WHERE login = %s", (login,))
        conn.commit()

async def handle_echo(reader, writer):
    """Fonction principale du serveur, appelée lors de la connexion avec un client. Après avoir exécuté la fonction log(), elle
    envoie ou récupère un projet selon les demandes de l'utilisateur"""
    action = 1
    while action:
        action = await reader.read(1)
        await log(action, writer, reader)
    connexion = True
    while connexion:
        action = await reader.read(1) #Variable pour savoir ce que l'utilisateur veut faire
        action = int.from_bytes(action, "big")
        print("action = ", action)
        if action == 0: #S'il souhaite envoyer un projet
            projectName = await receive_message(reader)
            projectName = projectName.decode()

            #On vérifie d'abord que le nom du projet est disponible
            cur.execute("SELECT nom FROM projet WHERE nom = %s", (projectName,))
            projectName_invalid = cur.fetchone() != None
            while projectName_invalid:
                writer.write(int(0).to_bytes(1, "big")) #On envoie 0 pour dire que le nom n'est pas disponible
                print("Ce nom est déjà pris.")
                projectName = await receive_message(reader)
                projectName = projectName.decode()
                cur.execute("SELECT nom FROM projet WHERE nom = %s", (projectName,))
                projectName_invalid = cur.fetchone() != None
            writer.write(int(1).to_bytes(1, "big")) #On envoie 1 pour dire que le nom est disponible

            #Ensuite, on incrémente de 1 le dernier id enregistré dans la base pour créer un nouvel id
            cur.execute("SELECT MAX(idProjet) FROM Projet;")
            data = cur.fetchone()
            if data[0] == None:
                new_id = 1
            else:
                new_id = data[0]+1
                
            #On récupère ensuite le chemin du projet ainsi que le login de l'utilisateur
            pathServer = await receive_message(reader)
            pathServer = pathServer.decode()
            user = await receive_message(reader)
            user = user.decode()
            
            cur.execute("SELECT idLog FROM UserInfo WHERE login = %s;", (user,)) #On récupère l'id de l'utilisateur
            idlog = cur.fetchone()[0]

            #Enfin, on insère dans la table Projet une nouvelle ligne, avec les informations du nouveau projet
            cur.execute("INSERT INTO Projet VALUES(%s, %s, %s, FALSE, %s, 'None', 1, 1);", (new_id, pathServer, idlog, projectName))

            #Pour finir, on récupère le projet en lui-même
            print("Réception d'un fichier client")
            downloading = await reader.read(1) #Variable pour savoir s'il reste des fichiers à recevoir
            downloading = int.from_bytes(downloading, "big")
            while downloading == 0:
                await receive_file(reader)
                downloading = await reader.read(1)
                downloading = int.from_bytes(downloading, "big")
                print("Downloading = ", downloading)
            print("Project downloaded")
            conn.commit()

        elif action == 1: #Si l'utilisateur souhaite télécharger un projet
            print("Envoi d'un fichier au client")
            idProjet = await reader.read(1) #On récupère le chemin du fichier qu'il souhaite récupérer
            idProjet = int.from_bytes(idProjet, 'big')
            cur.execute("SELECT nom, chemin FROM Projet WHERE idProjet = %s;", (idProjet,))
            data = cur.fetchone()
            path = data[1]
            print(f"\033[36mClient ask for {data[0]}\033[0m")
            oDir = path.split('/')[-1]
            await send_dir(path, writer, oDir)
            writer.write(b'\x00')
            print('dossier envoye')

        elif action == 2:
            connexion = False

        elif action == 3:
            await send_info_ProjetValides(writer, reader)
        
        else:
            print("Error action.")

        await writer.drain()
    print("Close the connection")
    writer.close()


async def main(): #Première fonction, qui "allume" le serveur
    server = await asyncio.start_server(
        handle_echo, '127.0.0.1', 50001)

    addr = server.sockets[0].getsockname()
    print(f'Serving on {addr}')

    async with server:
        await server.serve_forever()

asyncio.run(main())