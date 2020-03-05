#!/usr/bin/env python3.8
import psycopg2
from argon2 import *
import sys

conn = psycopg2.connect("dbname=paulbunel user=paulbunel")
cur = conn.cursor()
ph = PasswordHasher()

def register():
    register = True
    while register:
        login = input("Entrez un login : ")
        cur.execute("SELECT login FROM UserInfo WHERE login = %s;", (login,))
        if cur.fetchone() != None:
            print("\033[31mCe nom est déjà pris\033[0m")
        else:
            register = False

    password = input("Choisissez un mot de passe : ")
    cur.execute("SELECT MAX(idLog) FROM UserInfo;")
    data = cur.fetchone()
    if data[0] == None:
        new_id = 1
    else:
        new_id = data[0]+1
    mail = input("Entrez une adrese mail : ")
    cur.execute("INSERT INTO UserInfo VALUES(%s, %s, %s, %s)", (new_id, login, ph.hash(password), mail))
    conn.commit()
    print("Vous vous êtes correctement inscrit")

def login():
    connecting = True
    while connecting:
        login = input("Login : ")
        cur.execute("SELECT password FROM UserInfo WHERE login = %s", (login,))
        data = cur.fetchone()
        password = input("Mot de passe : ")
        try:
            ph.verify(data[0], password)
            connecting = False
            print("Bienvenue, ", login)
            delete_account(login)
        except (TypeError, exceptions.VerifyMismatchError):
            print("\033[31mLe nom d'utilisateur ou mot de passe est incorrect\033[0m")

def delete_account(login):
    action = input("Êtes-vous sûr de vouloir supprimer votre compte ? (y/n)")
    if action == 'y':
        cur.execute("DELETE FROM UserInfo WHERE login = %s", (login,))
        conn.commit()

action =''
while action != 'c' and action != 'd':
    action = input("Voulez-vous vous connecter ou vous inscrire ? (c/i) : ")
    if action == 'c':
        login()
    elif action == 'i':
        register()
    else:
        print("Veuillez entre 'c' pour vous connecter ou 'i' pour vous inscrire")

