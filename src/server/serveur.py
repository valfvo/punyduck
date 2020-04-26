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
    # print("Message = ", message, "sizeBuffer_send = ", sizeBuffer_send)

    return sizeBuffer_send

def decodeBuffer(buffer): #Fonction qui décode un nombre encodé en binaire (appelée pour décoder la taille d'un message)
    print("sizeBuffer_received : ", buffer)
    sizeBuffer_received = int.from_bytes(buffer, "big")
    # print("sizeBuffer_received : ", sizeBuffer_received)
    return sizeBuffer_received

async def send_file(path, writer): #Fonction envoyant un fichier au serveur. On passe en paramètre le chemin du fichier, le writer, et le dossier originel du projet
    writer.write(b'\x00') #On écrit un pour dire au client qu'il va recevoir un fichier
    try:
        with open(path, "rb") as file:
            """On crée un string "pathServer" qui stockera le chemin à partir du dossier originel. Cela sera utile dans les cas où
            le dossier à transmettre se situera à plusieurs dossiers d'intervalle du client, on supprimera donc les dossiers intervalles
            du chemin."""
            cur.execute("SELECT nom FROM Projet WHERE idProjet = %s", (path,))
            nom = cur.fetchone()[0]
            await send_message(writer, nom.encode())

            filesize = os.path.getsize(path)
            writer.write(filesize.to_bytes(4, 'big')) #On envoie la taille du fichier
            loop = asyncio.get_running_loop()
            await loop.sendfile(writer.transport, file) #On envoie le fichier
    except IOError: #Si le fichier n'a pas pu être ouvert
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert".encode()
        await send_message(writer, data)

async def receive_file(reader, oDir, name=None): #Fonction appelée pour recevoir un fichier envoyé par la fonction "send_file()" du serveur
    path = await receive_message(reader) #On récupère d'abord le chemin du fichier
    data = await receive_message(reader) #On récupère ensuite les données binaires du fichier
    path = path.decode()
    path = path.split('/')
    #Dans le chemin du fichier, on sépare le nom du fichier du reste du chemin
    nameFile = path[-1] if name == None else name
    path2 = oDir + '/'
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
        while i < len(message)/1024: #On envoie le message ko par ko si celui-ci est suffisament grand
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

async def send_dir(pathDir, writer):
    """Fonction envoyant les fichiers d'un dossier de manière récursive. On lui passe en paramètre le chemin pour accéder au dossier originel,
    le writer, et le nom du dossier originel."""
    print("send dir : ", pathDir)
    if os.path.isdir(pathDir):
        print(pathDir, "est un dossier")
        tree = os.listdir(pathDir)
        for fileordir in tree: #Pour chaque fichier/dossier du dossier originel :
            print("dir : ", pathDir+'/'+fileordir)
            await send_dir(pathDir+'/'+fileordir, writer)
            await writer.drain()
    elif os.path.isfile(pathDir):
        print(pathDir, "est un fichier")
        await send_file(pathDir, writer)
    else:
        print("Veuillez entrer un chemin valide")


async def register(writer, reader, infos):
    """Fonction appelée en premier lors de la connexion avec un client. Celle-ci gère la connexion du client à son compte,
    en effectuant les requêtes SQL adéquates. Elle prend en paramètre action, qui est un booléen (0/1) envoyé par le client
    pour indiquer s'il souhaite se connecter ou s'inscrire, ainsi que le writer et le reader."""
    #On récupère le login et le mot de passe saisis par le client
    print("Inscription en cours")
    infos = infos[1:].decode()
    print("Infos = ", infos)
    matchInfos = infos.split('|')
    login = matchInfos[0]
    password = matchInfos[1]
    email = matchInfos[2]
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


async def login(writer, reader, infos):
    print("Connexion en cours")
    infos = infos[1:].decode()
    print("Infos = ", infos)
    matchInfos = infos.split('|')
    login = matchInfos[0]
    password = matchInfos[1]
    
    cur.execute("SELECT password FROM UserInfo WHERE login = %s", (login,)) #On récupère le mot de passe correspondant au login
    data = cur.fetchone()
    
    try:
        ph.verify(data[0], password) #On vérifie que le mot de passe saisie corresponde à celui récupéré (qui est hashé)
        print("Bienvenue, ", login)
        cur.execute("SELECT idLog FROM UserInfo WHERE login = %s", (login,))
        idLog = cur.fetchone()[0]
        writer.write(idLog.to_bytes(2, 'big')) #On envoie au client le login pour lui indiquer qu'il s'est connecté
        return True
    except (TypeError, exceptions.VerifyMismatchError): #Si les identifiants sont incorrects, on en reçoit de nouveaux
        writer.write(b'\x00\x00') #On envoie "False" pour signifier au client que les identifiants sont incorrects
        return False


def checkProjectName(nom):
    print("Vérification du nom de projet")
    cur.execute("SELECT nom FROM Projet WHERE nom = %s", (nom,))
    return not (cur.fetchone() != None or nom == "Images")

async def getProject(writer, reader, idProjet):
    print("Réception d'un projet client")
    downloading = await reader.read(1) # Variable pour savoir s'il reste des fichiers à recevoir
    downloading = int.from_bytes(downloading, "big")
    print("Downloading = ", downloading)
    ddl = False
    while downloading == 0:
        await receive_file(reader, str(idProjet))
        downloading = await reader.read(1)
        downloading = int.from_bytes(downloading, "big")
        print("Downloading = ", downloading)
        ddl = True
    print("Project downloaded")
    return ddl
   

def delete_account(login): #Fonction appelée pour supprimer un compte
    action = input("Êtes-vous sûr de vouloir supprimer votre compte ? (y/n)")
    if action == 'y':
        cur.execute("DELETE FROM UserInfo WHERE login = %s", (login,))
        conn.commit()

idColonnes = {
    "idProjet": 0,
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

GL_RGB = b'\x19\x07'
GL_RGBA = b'\x19\x08'

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
        if match != None:
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
                        infos += img.tobytes()
                        # infos += "donneebinaireimage".encode()
                    else:
                        infos += data[row].encode() + b'\0'
                nItems += 1

        infos = "dataReceive".encode() + b'\0' + model.encode() + b'\0' + \
                nItems.to_bytes(4, 'big') + nAttributes + ordreIndice + infos
        # str + \0 + str + \0 + 4 bytes + 1 byte + (4 bytes + 4 * nbIndice bytes) + n bytes
        print("nitem :", nItems.to_bytes(4, 'big'))
        print("infos =", infos)
        await send_message(writer, infos)



async def handle_echo(reader, writer):
    """Fonction principale du serveur, appelée lors de la connexion avec un client. Après avoir exécuté la fonction log(), elle
    envoie ou récupère un projet selon les demandes de l'utilisateur"""
    connexion = True
    while connexion:
        # try:
        # action = await receive_message(reader) #Variable pour savoir ce que l'utilisateur veut faire
        action = await receive_message(reader)
        print("action = ", action) if len(action) < 300 else "erreur action"
        if action == 0: #Se déconnecter
            connexion = False

        elif action[0] == 1:
            print("Requête demandée")
            query = action[1:].decode()
            await SQL(writer, query)
        
        elif action[0] == 2:
            result = await login(writer, reader, action)
            if not result:
                print("Identifiants incorrects")
                action = await receive_message(reader)
                result = await login(writer, reader, action)

        elif action[0] == 3:
            result = await register(writer, reader, action)
            print("result : ", result)
            if not result:
                writer.write(b'\x00')
            else:
                writer.write(b'\x01')

        elif action[0] == 4:
            #On vérifie d'abord que le nom du projet est disponible
            infos = action[1:].decode()
            matchInfos = infos.split('|')
            nom = matchInfos[1]
            checkName = checkProjectName(nom)
            if not checkName:
                writer.write(b'\x00') #On envoie 0 pour dire que le nom n'est pas disponible
                print("Ce nom est déjà pris.")
            else:
                print("Nom correct")
                writer.write(b'\x01') #On envoie 1 pour dire que le nom est disponible
                tag = matchInfos[2]
                descr = matchInfos[3]
                clientPathImage = matchInfos[4]
                cur.execute("SELECT MAX(idProjet) FROM Projet;")
                idP = cur.fetchone()
                if idP[0] == None:
                    idP = 1
                else:
                    idP = idP[0] + 1

                ddl = await getProject(writer, reader, idP)
                
                #On récupère ensuite le chemin du projet ainsi que le login de l'utilisateur
                idLog = await reader.read(2)
                print("Project saved by idLog : ", idLog)
                idLog = int.from_bytes(idLog, 'big')

                print("Réception Image")
                if clientPathImage != "":
                    pathImage = str(idP)+"/PP"
                    await reader.read(1)
                    await receive_file(reader, pathImage, "pp.png")
                else:
                    pathImage = "PP/defaultpp.png"
                
                #Enfin, on insère dans la table Projet une nouvelle ligne, avec les informations du nouveau projet
                if ddl:
                    print("Adding project to database...")
                    cur.execute("INSERT INTO Projet (nom, valide, tag, pDescr, pPathImage, pIdLog)\
                    VALUES(%s, FALSE, %s, %s, %s, %s);", (nom, tag, descr, pathImage+"/pp.png", idLog))
                conn.commit()
                print("END")


        elif action[0] == 5: #Si l'utilisateur souhaite télécharger un projet
            print("Send project to client")
            idProjet = int.from_bytes(action[1:], 'big')
            print("idProjet : ", idProjet)
            path = str(idProjet)
            await send_dir(path, writer)
            writer.write(b'\x01')
            print('projet envoye')

        else:
            print("Error action.")

        await writer.drain()
        # except:
            # pass
    print("Close the connection")
    writer.close()


async def main(): #Première fonction, qui "allume" le serveur
    server = await asyncio.start_server(handle_echo, '127.0.0.1', 50002)

    addr = server.sockets[0].getsockname()
    print(f'Serving on {addr}')

    async with server:
        await server.serve_forever()

asyncio.run(main())