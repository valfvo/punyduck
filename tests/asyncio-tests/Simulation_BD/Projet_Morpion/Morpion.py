#Projet Morpion_alpha

#Importation de modules :
from tkinter import *
from tkinter.messagebox import *
import random

#Création de la fenêtre
fenetre = Tk()
fenetre["bg"] = "white"
fenetre.title("Morpion_alpha")
fenetre.geometry("1366x768")

jour = True
nuit = False

J1 = True
J2 = False
#Création d'un tableau qui ne sera pas affiché mais qui représentera notre grille dans les fonctions qui vérifieront les scores.
tableau_grille = [
[0, 0, 0],
[0, 0, 0],
[0, 0, 0]]

#Idem, mais celui-ci représentera notre grille tournée sur le côté (la première ligne du tableau = la première colonne de la grille)
tableau_grille2 = [
[0, 0, 0],
[0, 0, 0],
[0, 0, 0]]

score_J1 = 0
score_J2 = 0


def retour_accueil():
	"""Fonction qui détruit tout le contenu de la fenêtre et appelle la fonction "menu_jouer" pour tout recréer. Elle remet également
	les scores à 0."""
	global score_J1
	global score_J2
	global J1, J2
	score_J1 = 0
	score_J2 = 0
	J1 = True
	J2 = False
	global tableau_grille
	global tableau_grille2
	tableau_grille = [
	[0, 0, 0],
	[0, 0, 0],
	[0, 0, 0]]
	tableau_grille2 = [
	[0, 0, 0],
	[0, 0, 0],
	[0, 0, 0]]
	for c in fenetre.winfo_children():
		c.destroy()
	menu_jouer()

def changer_mode_morpion():
	"""Fonction appelée par le bouton mode de la fenêtre de morpion, qui a pour but de changer les couleurs entre un mode nuit et un
	mode jour."""
	global jour
	global nuit
	global barre_menu
	global score
	if jour == False and nuit == True: #On remplace toutes les couleurs par leur version "jour"
		fenetre["bg"] = "white"
		for c in grille_jeu.winfo_children():
			if c["bg"] == "black":
				c["bg"] = "white"
			if c["highlightbackground"] == "black":
				c["highlightbackground"] = "white"
		barre_menu["bg"] = "white"
		score["bg"] = "white"
		scoreJ1["bg"] = "white"
		scoreJ2["bg"] = "white"
		grille_jeu["bg"] = "white"
		grille_jeu.itemconfigure("lignes", fill = "black")
		jour = True
		nuit = False
	elif nuit == False and jour == True: #On remplace toutes les couleurs par leur version "nuit"
		fenetre["bg"] = "black"
		for c in grille_jeu.winfo_children():
			if c["bg"] == "white":
				c["bg"] = "black"
			if c["highlightbackground"] == "white":
				c["highlightbackground"] = "black"
		barre_menu["bg"] = "black"
		score["bg"] = "black"
		scoreJ1["bg"] = "black"
		scoreJ2["bg"] = "black"
		grille_jeu["bg"] = "black"
		grille_jeu.itemconfigure("lignes", fill = "white")
		jour = False
		nuit = True

def set_mode_morpion():
	"""Fonction avec le même but que celle du dessus mais qui cette fois est appelée à chaque fois que les couleurs ont besoin
	d'être actualisées (création de nouveaux widget)."""
	global jour
	global nuit
	global barre_nuit
	global score
	if jour == True and nuit == False: #On remplace toutes les couleurs par leur version "jour"
		for c in grille_jeu.winfo_children():
			if c["bg"] == "black":
				c["bg"] = "white"
			if c["highlightbackground"] == "black":
				c["highlightbackground"] = "white"
		barre_menu["bg"] = "white"
		score["bg"] = "white"
		scoreJ1["bg"] = "white"
		scoreJ2["bg"] = "white"
		grille_jeu["bg"] = "white"
		grille_jeu.itemconfigure("lignes", fill = "black")
	elif nuit == True and jour == False: #On remplace toutes les couleurs par leur version "nuit"
		for c in grille_jeu.winfo_children():
			if c["bg"] == "white":
				c["bg"] = "black"
			if c["highlightbackground"] == "white":
				c["highlightbackground"] = "black"
		barre_menu["bg"] = "black"
		score["bg"] = "black"
		scoreJ1["bg"] = "black"
		scoreJ2["bg"] = "black"
		grille_jeu["bg"] = "black"
		grille_jeu.itemconfigure("lignes", fill = "white")

#A partir d'ici sont créées les fonctions servant à la construction du jeu de morpion et de son interface graphique
def menu_morpion():
	global scoreJ1
	global scoreJ2
	global score
	global barre_menu
	#Création d'une Frame dans laquelle on placera les boutons mode et quitter :
	barre_menu = Frame(fenetre, bg = "white")
	barre_menu.pack(fill = "both", pady = 1)

	fermer = Button(barre_menu, bitmap = "@cross.xbm", width = 30, height = 30, command = retour_accueil)
	fermer.pack(side = "right", padx = 20)
	mode = Button(barre_menu, width = 4, height = 2, command = changer_mode_morpion)
	mode.pack(side = "right")

	#Création d'une Frame dans laquelle on placera les labels qui afficheront le score des joueurs :
	score = Frame(fenetre, bg = "white")
	score.pack()

	scoreJ1 = Label(score, text = "J1 : %s"%score_J1, fg = "blue", font = ('Comic Sans MS', 25), bg = "white")
	scoreJ1.pack(side = "left", padx = 190, pady = 5)

	scoreJ2 = Label(score, text = "J2 : %s"%score_J2, fg = "red", font = ('Comic Sans MS', 25), bg = "white")
	scoreJ2.pack(side = "right", padx = 230, pady = 5)

def afficher_rond():
	"""Fonction qui crée le pion "rond" du joueur 2."""
	global rond
	rond = Canvas(grille_jeu, width = 170, height = 170, bg = "white", highlightbackground = "white")
	rond.create_oval(10, 10, 160, 160, width = 10, outline = "red")

def afficher_croix():
	"""Fonction qui crée le pion "croix" du joueur 1."""
	global croix
	croix = Canvas(grille_jeu, width = 170, height = 170, bg = "white", highlightbackground = "white")
	croix.create_line(10, 10, 160, 160, width = 10, fill ="blue", capstyle = "round")
	croix.create_line(160, 10, 10, 160, width = 10, fill = "blue", capstyle = "round")

def rejouer():
	"""Fonction appelée lorsqu'on décide de rejouer à la fin d'une partie. Elle réinitialise les pions sur la grille."""
	global tableau_grille
	global tableau_grille2
	tableau_grille = [
	[0, 0, 0],
	[0, 0, 0],
	[0, 0, 0]]
	tableau_grille2 = [
	[0, 0, 0],
	[0, 0, 0],
	[0, 0, 0]]
	grille_jeu.destroy()
	grille_morpion()
	set_mode_morpion()

def ask_replay():
	"""Fonction appelée à la fin d'une partie, qui ouvre une boite de dialogue indiquant le vainqueur (ou l'égalité) et proposant
	au joueur de refaire une partie."""
	global score_J1
	global score_J2
	if p == 9:
		victoire = "Egalité !"
	elif J1 == True and J2 == False:
		victoire = "Victoire J1 !"
	elif J2 == True and J1 == False:
		if IA == True:
			victoire = "Victoire de l'ordinateur !"
		else:
			victoire = "Victoire J2 !"

	#On se sert du sous module "messagebox" de tkinter pour ouvrir une fenêtre de dialogue.
	replay = askokcancel("Rejouer ?", "%s Voulez-vous refaire une partie ?"%victoire, parent = fenetre)
	if replay == True:
		rejouer()
	else:
		score_J1 = 0
		score_J2 = 0
		retour_accueil()

def verify():
	"""Fonction qui vérifie après chaque coup si la partie est finie ou pas, en vérifiant si une ligne, une colonne, une diagonale
	ou même la grille entière n'est pas remplie."""
	global tableau_grille
	global J1
	global J2
	global scoreJ1
	global scoreJ2
	global score_J1
	global score_J2
	global p

	#On vérifie qu'aucune ligne d'un même pion n'est remplie.
	for p in tableau_grille:
		if p == [1, 1, 1]:
			score_J1 += 1
			scoreJ1["text"] = "J1 : %s"%score_J1
			J1 = True
			J2 = False
			ask_replay()

		elif p == [2, 2, 2]:
			score_J2 += 1
			scoreJ2["text"] = "J2 : %s"%score_J2
			J1 = False
			J2 = True
			ask_replay()

	#On vérifie qu'aucune colonne d'un même pion n'est remplie.
	for p in tableau_grille2:
		if p == [1, 1, 1]:
			score_J1 += 1
			scoreJ1["text"] = "J1 : %s"%score_J1
			J1 = True
			J2 = False
			ask_replay()

		elif p == [2, 2, 2]:
			score_J2 += 1
			scoreJ2["text"] = "J2 : %s"%score_J2
			J1 = False
			J2 = True
			ask_replay()

	#On vérifie qu'auncune diagonale n'est remplie d'un même pion.
	if tableau_grille[0][0] != 0 and tableau_grille[1][1] != 0 and tableau_grille[2][2] == tableau_grille[0][0] == tableau_grille[1][1]:
		if tableau_grille[0][0] == 1:
				score_J1 += 1
				scoreJ1["text"] = "J1 : %s"%score_J1
				J1 = True
				J2 = False
		elif tableau_grille[0][0] == 2:
				score_J2 += 1
				scoreJ2["text"] = "J2 : %s"%score_J2
				J1 = False
				J2 = True
		ask_replay()

	if tableau_grille[0][2] != 0 and tableau_grille[1][1] != 0 and tableau_grille[2][0] == tableau_grille[0][2] == tableau_grille[1][1]:
		if tableau_grille[0][2] == 1:
				score_J1 += 1
				scoreJ1["text"] = "J1 : %s"%score_J1
				J1 = True
				J2 = False
		elif tableau_grille[0][2] == 2:
				score_J2 += 1
				scoreJ2["text"] = "J2 : %s"%score_J2
				J1 = False
				J2 = True
		ask_replay()

	#On vérifie que la grille n'est pas pleine (ce qui entrainerai une égalité).
	p = 0
	for i in range(3):
		for j in range(3):
			if tableau_grille[i][j] != 0:
				p += 1
				if p == 9:
					ask_replay()

def clique(bouton):
	"""Fonction appelée par les boutons qui constituent les cases de la grille, qui appelle soit la fonction "clique" du morpion joueur
	contre joueur soit celle du morpion joueur contre IA."""
	global IA
	if IA == True:
		clique_IA(bouton)
	else:
		clique_jcj(bouton)

def clique_jcj(bouton):
	"""Fonction clique du morpion joueur contre joueur, qui pose le pion croix ou rond en fonction du joueur et qui appelle la
	fonction "verify". Elle prend en paramètre la méthode "grid_info" du bouton sur lequel le joueur a clique, ce qui lui indique
	les différentes caractéristiques du placement du bouton sur la grille."""
	global J
	global J1
	global J2
	global tableau_grille
	global tableau_grille2
	coordonnees = bouton.grid_info()
	#On récupère les coordonnées du bouton : l pour la ligne, c pour la colonne.
	l = coordonnees['row']
	c = coordonnees['column']

	#On pose le pion nécessaire et on l'indique également dans les liste tableau_grille et tableau_grille2.
	if J1 == True and J2 == False:
		J = 1
		tableau_grille[l][c] = J
		tableau_grille2[c][l] = J
		J1 = False
		J2 = True
		afficher_croix()
		croix.grid(row = l, column = c, padx = 19, pady = 19, in_ = grille_jeu)
	elif J1 == False and J2 == True:
		J = 2
		tableau_grille[l][c] = J
		tableau_grille2[c][l] = J
		J1 = True
		J2 = False
		afficher_rond()
		rond.grid(row = l, column = c, padx = 19, pady = 19, in_ = grille_jeu)

	bouton.destroy()
	set_mode_morpion()
	verify()

def clique_IA(bouton):
	"""Même fonction que au dessus mais pour le mode morpion contre IA. IA est un grand mot puisque l'ordinateur se contente de placer
	des pions de manière aléatoire (faute de temps pour nous). A chaque clic du joueur, son pion est posé sur le bouton cliqué et l'autre
	pion est posé aléatoirement."""
	global J
	global tableau_grille
	global tableau_grille2
	coordonnees = bouton.grid_info()
	l = coordonnees['row']
	c = coordonnees['column']

	J = 1
	tableau_grille[l][c] = J
	tableau_grille2[c][l] = J
	afficher_croix()
	croix.grid(row = l, column = c, padx = 19, pady = 19, in_ = grille_jeu)
	set_mode_morpion()
	verify()
	J = 2
	case_libre = False
	while case_libre == False:
		l = random.randrange(3)
		c = random.randrange(3)
		if tableau_grille[l][c] != 0 or tableau_grille2[c][l] != 0:
			case_libre = False
		else:
			tableau_grille[l][c] = J
			tableau_grille2[c][l] = J
			case_libre = True
	"""On tente de supprimer le bouton sur lequel l'ordinateur pose son pion puis de créer et d'afficher le pion. On a met ces
	instructions sous un "try" pour éviter des bugs en fin de partie."""
	try:
		bouton_deux = grille_jeu.grid_slaves(row = l, column = c)
		bouton_deux[0].destroy()
		afficher_rond()
		rond.grid(row = l, column = c, padx = 19, pady = 19, in_ = grille_jeu)
		set_mode_morpion()
	except:
		pass
	
	bouton.destroy()
	verify()

def grille_morpion():
	"""Fonction qui s'occupe de créer la grille du morpion. On place 9 boutons dans un grand Canvas."""
	global grille_jeu
	#Création d'un Canvas qui représente la grille entière :
	grille_jeu = Canvas(fenetre, bg = "white", highlightbackground = "white")
	grille_jeu.pack()

	"""Création de 9 boutons dans le premier Canvas qui représentent chacun une case du morpion. Nous n'avons pas réussi à
	les créer autrement que un à un."""
	bouton1 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton1))
	bouton1.grid(row = 0, column = 0, padx = 5, pady = 9, in_ = grille_jeu)
	bouton2 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton2))
	bouton2.grid(row = 0, column = 1, padx = 5, pady = 9, in_ = grille_jeu)
	bouton3 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton3))
	bouton3.grid(row = 0, column = 2, padx = 5, pady = 9, in_ = grille_jeu)
	bouton4 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton4))
	bouton4.grid(row = 1, column = 0, padx = 5, pady = 9, in_ = grille_jeu)
	bouton5 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton5))
	bouton5.grid(row = 1, column = 1, padx = 5, pady = 9, in_ = grille_jeu)
	bouton6 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton6))
	bouton6.grid(row = 1, column = 2, padx = 5, pady = 9, in_ = grille_jeu)
	bouton7 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton7))
	bouton7.grid(row = 2, column = 0, padx = 5, pady = 9, in_ = grille_jeu)
	bouton8 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton8))
	bouton8.grid(row = 2, column = 1, padx = 5, pady = 9, in_ = grille_jeu)
	bouton9 = Button(grille_jeu, width = 24, height = 9, bg = "white", relief = "flat", command = lambda : clique(bouton9))
	bouton9.grid(row = 2, column = 2, padx = 5, pady = 9, in_ = grille_jeu)

	#Création des lignes séparant les cases
	grille_jeu.create_line(212, 0, 212, 650, width = 5, tags = "lignes")
	grille_jeu.create_line(423, 0, 423, 650, width = 5, tags = "lignes")
	grille_jeu.create_line(0, 212, 650, 212, width = 5, tags = "lignes")
	grille_jeu.create_line(0, 423, 650, 423, width = 5, tags = "lignes")


def start_morpion():
	"""Fonction appelée par les boutons qui lancent une partie de morpion joueur contre joueur."""
	global IA
	IA = False
	for c in fenetre.winfo_children():
		c.destroy()
	menu_morpion()
	grille_morpion()
	set_mode_morpion()

def start_morpion_IA():
	"""Fonction appelée par les boutons qui lancent une partie de morpion joueur contre IA."""
	global IA
	IA = True
	for c in fenetre.winfo_children():
		c.destroy()
	menu_morpion()
	grille_morpion()
	set_mode_morpion()


#A partir d'ici les fonctions créées servent à la construction du menu d'accueil
def changer_mode_couleur():
	"""Fonction qui sert à changer les couleurs de la fenêtre (mode jour/mode nuit)"""
	global jour
	global nuit
	if jour == True and nuit == False: #On remplace toutes les couleurs par leur version "nuit"
		fenetre["bg"] = "black"
		titre["fg"] = "white"
		titre["bg"] = "black"
		menu_bouton["bg"] = "darkblue"
		for c in menu_bouton.winfo_children():
			if c["bg"] == "red":
				c["bg"] = "red3"
			if c["fg"] == "cyan":
				c["fg"] = "darkblue"
		jour = False
		nuit = True

	elif nuit == True and jour == False: #On remplace toutes les couleurs par leur version "jour"
		fenetre["bg"] = "white"
		titre["fg"] = "black"
		titre["bg"] = "white"
		menu_bouton["bg"] = "cyan"
		for c in menu_bouton.winfo_children():
			if c["bg"] == "red3":
				c["bg"] = "red"
			if c["fg"] == "darkblue":
				c["fg"] = "cyan"
		jour = True
		nuit = False

def set_mode_couleur():
	"""Fonction permettant "d'actualiser" les couleurs à chaque création de nouveaux éléments pour éviter que ces derniers aient une
	couleur différente des autres"""
	global jour
	global nuit
	if nuit == True and jour == False:
		fenetre["bg"] = "black"
		titre["fg"] = "white"
		titre["bg"] = "black"
		menu_bouton["bg"] = "darkblue"
		for c in menu_bouton.winfo_children():
			if c["bg"] == "red":
				c["bg"] = "red3"
			if c["fg"] == "cyan":
				c["fg"] = "darkblue"

	elif jour == True and nuit == False:
		fenetre["bg"] = "white"
		titre["fg"] = "black"
		titre["bg"] = "white"
		menu_bouton["bg"] = "cyan"
		for c in menu_bouton.winfo_children():
			if c["bg"] == "red3":
				c["bg"] = "red"
			if c["fg"] == "darkblue":
				c["fg"] = "cyan"

def menu_jouer():
	"""Fonction appelée au lancement de la fenêtre, qui a pour rôle de créer l'ensemble du menu d'accueil à son lancement"""
	global menu_bouton
	global titre

	#Création du titre
	titre = Label(fenetre, text = "Jeu du Morpion", bg = "white", font = ("Comic Sans MS", 30), fg = "black")
	titre.pack(pady = 50)

	#Création d'une Frame dans laquelle on mettra les boutons
	menu_bouton = Frame(fenetre, borderwidth = 2, relief = RIDGE, bg = "cyan", highlightthickness = 0)
	menu_bouton.pack(ipadx = 5, ipady = 5)

	#Création des boutons
	jvj = Button(menu_bouton, text = "Joueur contre Joueur", width = 30, height = 2, bg = "red", font = ("Comic Sans MS", 20), fg = "cyan",
		command = start_morpion)
	jvia = Button(menu_bouton, text = "Joueur contre IA", width = 30, height = 2, bg = "red", font = ("Comic Sans MS", 20), fg = "cyan",
		command = start_morpion_IA)
	quitter = Button(menu_bouton, text = "Quitter", width = 30, height = 2, bg = "red", font = ("Comic Sans MS", 20), fg = "cyan",
		command = fenetre.destroy)
	mode = Button(menu_bouton, width = 9, height = 4, bg = "red", command = changer_mode_couleur)

	#Affichage des boutons
	jvj.pack(padx = 15, pady = 17)
	jvia.pack(padx = 15, pady = 17)
	quitter.pack(padx = 15, pady = 17)
	mode.pack(side = "left", pady = 5, padx = 285)
	set_mode_couleur()

menu_jouer()
fenetre.mainloop()