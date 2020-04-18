#!/usr/bin/env python3.8
import asyncio
import os
import re
import psycopg2
from argon2 import *
import sys
from PIL import Image

#Création du connecteur à la base de donnée, de son curseur et du hasher de mot de passe
conn = psycopg2.connect("dbname=paulbunel user=paulbunel")
cur = conn.cursor()
ph = PasswordHasher()
conn.autocommit = True


def encodeBuffer(message): #Fonction qui renvoie la taille d'un message en binaire
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("Message = ", message, "sizeBuffer_send = ", sizeBuffer_send)

    return sizeBuffer_send

def decodeBuffer(buffer): #Fonction qui décode un nombre encodé en binaire (appelée pour décoder la taille d'un message)
    sizeBuffer_received = int.from_bytes(buffer, "big")
    print("sizeBuffer_received : ", sizeBuffer_received)
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

# async def send_info_ProjetValides(writer, reader, n, nread):
#     cur.execute("SELECT idLog, nom, tag, Descr, pathImage FROM Projet LIMIT %s OFFSET %s;", (n, nread))
#     datas = cur.fetchall()
#     infos = "resp".encode()
#     for data in datas:
#         print(data)
#         nom = data[1]
#         descr = data[3]
#         with open(data[4], 'rb') as file:
#             # img = file.read()
#             img = "none".encode()
#             infos += (len(nom).to_bytes(1, 'big')
#                     + len(descr).to_bytes(2, 'big')
#                     + len(img).to_bytes(3, 'big')
#                     + nom.encode() + descr.encode() + img)
#     await send_message(writer, infos)

async def register(writer, reader, infos):
    """Fonction appelée en premier lors de la connexion avec un client. Celle-ci gère la connexion du client à son compte,
    en effectuant les requêtes SQL adéquates. Elle prend en paramètre action, qui est un booléen (0/1) envoyé par le client
    pour indiquer s'il souhaite se connecter ou s'inscrire, ainsi que le writer et le reader."""
    #On récupère le login et le mot de passe saisis par le client
    print("Inscription en cours")
    infos = infos[1:].decode()
    print("Infos = ", infos)
    matchInfos = re.match(r'(.*)\|(.*)\|(.*)', infos)
    login = matchInfos.group(1)
    password = matchInfos.group(2)
    email = matchInfos.group(3)
    print("login = ", login)
    print("password = ", password)
    print("email = ", email)
    query = "INSERT INTO UserInfo (login, password, email, admin, uPathImage, uDescr) VALUES('%s', '%s', '%s', FALSE, 'Images/defaultpp.png', 'Cette page na pas de description')" % (login, ph.hash(password), email)
    
    try:
        cur.execute(query)
        print("Pas d'erreur")
        return True
    except Exception as e:
        print(e)
        print("Login invalide")
        return False


async def login(writer, reader, action):
    login = await receive_message(reader)
    login = login.decode()
    password = await receive_message(reader)
    password = password.decode()
    incorrectLog = True
    while incorrectLog: #Tant que les identifiants sont incorrects
        cur.execute("SELECT password FROM UserInfo WHERE login = %s", (login,)) #On récupère le mot de passe correspondant au login
        data = cur.fetchone()
        print("data = ", data)
        try:
            ph.verify(data[0], password) #On vérifie que le mot de passe saisie corresponde à celui récupéré (qui est hashé)
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

idColonnes = {
    "idProjet": 0,
    "chemin": 1,
    "valide": 2,
    "nom": 3,
    "tag": 4,
    "pDescr": 5,
    "pPathImage": 6,
    "pIdLog": 7,
    "idLog": 8,
    "login": 9,
    "password": 10,
    "email": 11,
    "admin": 12,
    "uPathImage": 13,
    "uDescr": 14
}

GL_RGB = b'\x00\x00\x19\x07'
GL_RGBA = b'\x00\x00\x19\x08'

def fusion(T1, T2, T):
    i1 = 0
    i2 = 0
    for i in range(len(T)):
        if i1 >= len(T1):
            T[i] = T2[i2]
            i2 += 1
        elif i2 >= len(T2):
            T[i] = T1[i1]
            i1 += 1
        elif idColonnes[T1[i1]] < idColonnes[T2[i2]]:
            T[i] = T1[i1]
            i1 += 1
        else:
            T[i] = T2[i2]
            i2 += 1

def triFusion(T):
    if len(T) > 1:
        T1 = T[:int(len(T)/2)]
        triFusion(T1)
        T2 = T[int(len(T)/2):]
        triFusion(T2)
        
        fusion(T1, T2, T)

nAttributes = int(15).to_bytes(1, 'big')

async def SQL(writer, query):
    model = re.search(r'(.*)SELECT', query).group(1)
    query = re.search(r'(SELECT.*)', query).group(1)

    infos = b''
    nItems = 0

    if "*" in query:
        cur.execute(query)
        datas = cur.fetchall()

        ordreIndice = int(0).to_bytes(4, 'big')
        if "Projet" in query:
            for data in datas:
                for row in range(len(data)):
                    if row == 1 or row == 6:
                        # infos += int(10).to_bytes(4, 'big')
                        # infos += "1234567890".encode()
                        #img = width + height + format + len(data) + data
                        img = Image.open(data[row].replace("\\\\", "\\"))
                        infos += img.width.to_bytes(4, 'big')
                        infos += img.height.to_bytes(4, 'big')
                        infos += GL_RGBA if img.mode == "RGBA" else GL_RGB
                        infos += len(img.tobytes()).to_bytes(4, 'big')
                        infos += img.tobytes()
                    elif row == 0 or row == 7 or row == 2:
                        infos += data[row].to_bytes(4, 'big')
                    else:
                        infos += data[row].encode() + b'\0'
                nItems += 1

        elif "UserInfo" in query:
            for data in datas:
                for row in range(len(data)):
                    if row == 5:
                        #img = width + height + format + len(data) + data
                        img = Image.open(data[row])
                        infos += img.width.to_bytes(4, 'big')
                        infos += img.height.to_bytes(4, 'big')
                        infos += GL_RGBA if img.mode == "RGBA" else GL_RGB
                        infos += len(img.tobytes()).to_bytes(4, 'big')
                        infos += img.tobytes()
                    elif row == 0:
                        infos += data[row].to_bytes(4, 'big')
                    else:
                        infos += data[row].encode() + b'\0'
                nItems += 1
    
    else:
        match = re.search(r'^SELECT (.*) FROM .*;', query)
        rows = match.group(1).split(', ')
        endQuery = re.search(r'(FROM.*)', query).group(1)
        triFusion(rows)
        query = "SELECT "
        for row in rows[:-1]:
            query += row + ", "
        query += rows[-1] + " " + endQuery

        ordreIndice = len(rows).to_bytes(4, 'big')
        for row in rows:
            ordreIndice += idColonnes[row].to_bytes(4, 'big')

        cur.execute(query)
        datas = cur.fetchall()

        for data in datas:
            for row in range(len(data)):
                if idColonnes[rows[row]] == 0 or idColonnes[rows[row]] == 2 or idColonnes[rows[row]] == 7 or idColonnes[rows[row]] == 8:
                    infos += data[row].to_bytes(4, 'big')
                elif idColonnes[rows[row]] == 6 or idColonnes[rows[row]] == 13:
                    #img = width + height + format + len(data) + data
                    fp = open(data[row], 'rb')
                    img = Image.open(fp)
                    infos += img.width.to_bytes(4, 'big')
                    infos += img.height.to_bytes(4, 'big')
                    infos += GL_RGBA if img.mode == "RGBA" else GL_RGB
                    infos += len(img.tobytes()).to_bytes(4, 'big')
                    # infos += img.tobytes()
                    infos += "donneebinaireimage".encode()
                else:
                    infos += data[row].encode() + b'\0'
            nItems += 1

    infos = "dataReceive".encode() + b'\0' + model.encode() + nItems.to_bytes(4, 'big') + nAttributes + ordreIndice + infos
    print("infos = ", infos)
    await send_message(writer, infos)

async def handle_echo(reader, writer):
    """Fonction principale du serveur, appelée lors de la connexion avec un client. Après avoir exécuté la fonction log(), elle
    envoie ou récupère un projet selon les demandes de l'utilisateur"""
    connexion = True
    while connexion:
        # action = await receive_message(reader) #Variable pour savoir ce que l'utilisateur veut faire
        action = await receive_message(reader)
        # action = action.decode()
        print("action = ", action)
        # await asyncio.sleep(1000)
        if action == 0: #Se déconnecter
            connexion = False

        elif action[0] == 1:
            print("Requête demandée")
            query = action[1:].decode()
            await SQL(writer, query)
        
        elif action[0] == 2:
            await login(writer, reader, action)

        elif action[0] == 3:
            # task = asyncio.create_task(register(writer, reader, action))
            result = await register(writer, reader, action)
            print("result : ", result)
            while not result:
                writer.write(b'\x00')
                action = await receive_message(reader)
                result = await register(writer, reader, action)
                # task = asyncio.create_task(register(writer, reader, action))
            writer.write(b'\x01')



        # elif action == 2: #S'il souhaite envoyer un projet
        #     projectName = await receive_message(reader)
        #     projectName = projectName.decode()

        #     #On vérifie d'abord que le nom du projet est disponible
        #     cur.execute("SELECT nom FROM projet WHERE nom = %s", (projectName,))
        #     projectName_invalid = cur.fetchone() != None
        #     while projectName_invalid:
        #         writer.write(b'\x00') #On envoie 0 pour dire que le nom n'est pas disponible
        #         print("Ce nom est déjà pris.")
        #         projectName = await receive_message(reader)
        #         projectName = projectName.decode()
        #         cur.execute("SELECT nom FROM projet WHERE nom = %s", (projectName,))
        #         projectName_invalid = cur.fetchone() != None
        #     writer.write(b'\x01') #On envoie 1 pour dire que le nom est disponible

        #     #Ensuite, on incrémente de 1 le dernier id enregistré dans la base pour créer un nouvel id
        #     cur.execute("SELECT MAX(idProjet) FROM Projet;")
        #     data = cur.fetchone()
        #     if data[0] == None:
        #         new_id = 1
        #     else:
        #         new_id = data[0]+1
                
        #     #On récupère ensuite le chemin du projet ainsi que le login de l'utilisateur
        #     pathServer = await receive_message(reader)
        #     pathServer = pathServer.decode()
        #     user = await receive_message(reader)
        #     user = user.decode()
            
        #     cur.execute("SELECT idLog FROM UserInfo WHERE login = %s;", (user,)) #On récupère l'id de l'utilisateur
        #     idlog = cur.fetchone()[0]

        #     #Enfin, on insère dans la table Projet une nouvelle ligne, avec les informations du nouveau projet
        #     cur.execute("INSERT INTO Projet VALUES(%s, %s, %s, FALSE, %s, 'None', 1, 1);", (new_id, pathServer, idlog, projectName))

        #     #Pour finir, on récupère le projet en lui-même
        #     print("Réception d'un fichier client")
        #     downloading = await reader.read(1) #Variable pour savoir s'il reste des fichiers à recevoir
        #     downloading = int.from_bytes(downloading, "big")
        #     while downloading == 0:
        #         await receive_file(reader)
        #         downloading = await reader.read(1)
        #         downloading = int.from_bytes(downloading, "big")
        #         print("Downloading = ", downloading)
        #     print("Project downloaded")
        #     conn.commit()

        # elif action == 3: #Si l'utilisateur souhaite télécharger un projet
        #     print("Envoi d'un fichier au client")
        #     idProjet = await reader.read(1) #On récupère le chemin du fichier qu'il souhaite récupérer
        #     idProjet = int.from_bytes(idProjet, 'big')
        #     cur.execute("SELECT nom, chemin FROM Projet WHERE idProjet = %s;", (idProjet,))
        #     data = cur.fetchone()
        #     path = data[1]
        #     print(f"\033[36mClient ask for {data[0]}\033[0m")
        #     oDir = path.split('/')[-1]
        #     await send_dir(path, writer, oDir)
        #     writer.write(b'\x00')
        #     print('dossier envoye')

        # elif action == 4:
        #     await send_info_ProjetValides(writer, reader)

        # elif action == 5:
        #     connexion = False
        
        # else:
        #     print("Error action.")

        await writer.drain()
    print("Close the connection")
    writer.close()


async def main(): #Première fonction, qui "allume" le serveur
    server = await asyncio.start_server(handle_echo, '127.0.0.1', 50002)

    addr = server.sockets[0].getsockname()
    print(f'Serving on {addr}')

    async with server:
        await server.serve_forever()

asyncio.run(main())