# -*- coding: utf-8 -*-
"""
Created on Wed Apr  7 09:30:09 2021

@author: Thomas
"""

import json




###extraction des donnees de l'environnement

data_env = json.load(open('Description_Environnement_SansAccent_V10-05-17.js'))

unite = data_env["unite"]
#contours du terrain
liste_terrain = data_env["terrain_evolution"]
liste_terrain_coords = []
for i in range(len(liste_terrain)):
    liste_terrain_coords.append([liste_terrain[i]["point"]["x"],liste_terrain[i]["point"]["y"]]) #liste des coordonnees des limites du terrain
#print(liste_terrain_coords)
#coordonnees des differents obstacles dans des liste separees selon leur type
liste_obstacles = data_env["obstacles"]
liste_polygones_coords = []
liste_cercles_coords = [] #coords du centre puis rayon
indice_polygones = -1 #on initialise a -1 pour commencer à l'indice 0
indice_cercles = -1
for i in range(len(liste_obstacles)):
    if liste_obstacles[i]["type"] == 'polygone':
        indice_polygones = indice_polygones + 1
        liste_polygones_coords.append([])
        for j in range(len(liste_obstacles[i]["coordonnees"])):
            liste_polygones_coords[indice_polygones].append([liste_obstacles[i]["coordonnees"][j]["point"]["x"],liste_obstacles[i]["coordonnees"][j]["point"]["y"]])
    else: #alors c'est un cercle
        indice_cercles = indice_cercles + 1
        liste_cercles_coords.append([])
        liste_cercles_coords[indice_cercles].append([liste_obstacles[i]["coordonnees"][0]["centre"]["x"],liste_obstacles[i]["coordonnees"][0]["centre"]["y"]])
        liste_cercles_coords[indice_cercles].append(liste_obstacles[i]["coordonnees"][1]["rayon"])

#contours ou peut être la cible
liste_contours = data_env["contour_cible"]
liste_contours_coords = [] 
for i in range(len(liste_contours)):
    liste_contours_coords.append([liste_contours[i]["point"]["x"],liste_contours[i]["point"]["y"]])
#print(liste_contours_coords)
pos_cible = data_env["cible"]
pos_cible_coords = [] #coords du centre et hauteur
pos_cible_coords.append([pos_cible["centre"]["x"],pos_cible["centre"]["y"]])
pos_cible_coords.append(pos_cible["hauteur"])



###extraction des donnees de l'evolution

data_path = json.load(open('Evolution_Base_Roulante_SansAccent_V10-05-17.js'))

unite = data_path["unite"]
unite_angulaire = data_path["unite_angulaire"]
#position de depart
pos_depart = data_path["depart"]
pos_depart_coords = [] #coords de la pause de depart et angle
pos_depart_coords.append([pos_depart["coordonnees"]["x"],pos_depart["coordonnees"]["y"]])
pos_depart_coords.append(pos_depart["angle"])
#position d'arrivee
pos_arrivee = data_path["arrivee"]
pos_arrivee_coords = [] #coords de la pause d'arrivee et angle
pos_arrivee_coords.append([pos_arrivee["coordonnees"]["x"],pos_arrivee["coordonnees"]["y"]])
pos_arrivee_coords.append(pos_arrivee["angle"])
#etapes du parcours
liste_etapes = data_path["etapes"]
liste_etapes_coords = [] #coords des etapes et angles
nb_etapes = -1
for i in range(len(liste_etapes)):
    nb_etapes = nb_etapes + 1
    liste_etapes_coords.append([])
    liste_etapes_coords[nb_etapes].append([liste_etapes[i]["coordonnees"]["x"],liste_etapes[i]["coordonnees"]["y"]])
    liste_etapes_coords[nb_etapes].append(liste_etapes[i]["angle"])
nombre_arrets = len(liste_etapes) + 1 + 1  #arrivee et arret de tir
#print(nombre_arrets)
#position lors du tir sur la cible
pos_tir_cible = data_path["position-tir_cible"]
pos_tir_cible_coords = [] #coords et angle de la position lors du tir sur la cible
pos_tir_cible_coords.append([pos_tir_cible["coordonnees"]["x"],pos_tir_cible["coordonnees"]["y"]])
pos_tir_cible_coords.append(pos_tir_cible["angle"])

###modelisation de l'environnement avec une liste
#taille de la modélisation de carte
min_x = liste_contours_coords[0][0]
min_y = liste_contours_coords[0][1]
max_x = liste_contours_coords[0][0]
max_y = liste_contours_coords[0][1]
for i in range(len(liste_contours_coords)):
    if liste_contours_coords[i][0] > max_x:
        max_x = liste_contours_coords[i][0]
    if liste_contours_coords[i][1] > max_y:
        max_y = liste_contours_coords[i][1]
    if liste_contours_coords[i][0] < min_x:
        min_x = liste_contours_coords[i][0]
    if liste_contours_coords[i][1] < min_y:
        min_y = liste_contours_coords[i][1]
taille_grille_x = round((max_x - min_x)/25 + 1)
taille_grille_y = round((max_y - min_y)/25 + 1 )
grille = []
grille = [['0']*taille_grille_x for _ in range(taille_grille_y)]
#25 = diff de coord entre deux positions de liste
#remplissage de la grille
#premier point
x_c = round((liste_contours_coords[0][0] - min_x)/25)
y_c = round((max_y - liste_contours_coords[0][1])/25)
grille[y_c][x_c] = 'C'
#points suivants
#contours : fonctionnent
def contours(liste_coords, grille, x, y, lettre):
    for i in range(1,len(liste_coords)+1):
        if i == len(liste_coords):
            if liste_coords[i-1][0] == liste_coords[0][0]:
                if liste_coords[0][1] > liste_coords[i-1][1]: #on baisse les indices
                    ty = round((liste_coords[i-1][1] - liste_coords[0][1])/25)
                    for j in range(ty,0):
                        grille[y+j][x] = lettre
                    y = y + ty
                else: #on augmente les indices
                    ty = round((liste_coords[i-1][1] - liste_coords[0][1])/25)
                    for j in range(ty+1):
                        grille[y+j][x] = lettre
                    y = y + ty
            elif liste_coords[i-1][1] == liste_coords[0][1]:
                if liste_coords[0][0] > liste_coords[i-1][0]: #on augmente les indices
                    tx = round((liste_coords[0][0] - liste_coords[i-1][0])/25)
                    for j in range(tx+1):
                        grille[y][x+j] = lettre
                    x = x + tx
                else: #on baisse les indices
                    tx = round((liste_coords[0][0] - liste_coords[i-1][0])//25)
                    for j in range(tx,0):
                        grille[y][x+j] = lettre
                    x = x + tx
            elif (liste_coords[0][1]%25) == (liste_coords[i-1][1]%25): #pour les diagonales
                if liste_coords[0][0] > liste_coords[i-1][0]: #on augmente les indices
                    if liste_coords[0][1] > liste_coords[i-1][1]: #x> et y>
                        tx = round((liste_coords[0][0] - liste_coords[i-1][0])/25)
                        ty = round((liste_coords[i-1][1] - liste_coords[0][1])/25)
                        for j in range(tx+1):
                            grille[y-j][x+j] = lettre
                        x = x + tx
                        y = y + ty
                    else: #x> et y<
                        tx = round((liste_coords[0][0] - liste_coords[i-1][0])/25)
                        ty = round((liste_coords[i-1][1] - liste_coords[0][1])/25)
                        for j in range(tx+1):
                            grille[y+j][x+j] = lettre
                        x = x + tx
                        y = y + ty
                else: #on baisse les indices
                    if liste_coords[0][1] > liste_coords[i-1][1]: #x< et y>
                        tx = round((liste_coords[0][0] - liste_coords[i-1][0])/25)
                        ty = round((liste_coords[i-1][1] - liste_coords[0][1])/25)
                        for j in range(tx,0):
                            grille[y+j][x+j] = lettre
                        x = x + tx
                        y = y + ty
                    else: #x< et y<
                        tx = round((liste_coords[0][0] - liste_coords[i-1][0])/25)
                        ty = round((liste_coords[i-1][1] - liste_coords[0][1])/25)
                        for j in range(tx,0):
                            grille[y-j][x+j] = lettre
                        x = x + tx
                        y = y + ty
        else:
            if liste_coords[i][0] == liste_coords[i-1][0]:
                if liste_coords[i][1] > liste_coords[i-1][1]: #on baisse les indices
                    ty = round((liste_coords[i-1][1] - liste_coords[i][1])/25)
                    for j in range(ty,0):
                        grille[y+j][x] = lettre
                    y = y + ty
                else: #on augmente les indices
                    ty = round((liste_coords[i-1][1] - liste_coords[i][1])/25)
                    for j in range(ty+1):
                        grille[y+j][x] = lettre
                    y = y + ty
            elif liste_coords[i][1] == liste_coords[i-1][1]:
                if liste_coords[i][0] > liste_coords[i-1][0]: #on augmente les indices
                    tx = round((liste_coords[i][0] - liste_coords[i-1][0])/25)
                    for j in range(tx+1):
                        grille[y][x+j] = lettre
                    x = x + tx
                else: #on baisse les indices
                    tx = round((liste_coords[i][0] - liste_coords[i-1][0])/25)
                    for j in range(tx,0):
                        grille[y][x+j] = lettre
                    x = x + tx
            elif (liste_coords[i][1]%25) == (liste_coords[i-1][1]%25): #pour les diagonales
                if liste_coords[i][0] > liste_coords[i-1][0]: #on augmente les indices
                    if liste_coords[i][1] > liste_coords[i-1][1]: #x> et y>
                        tx = round((liste_coords[i][0] - liste_coords[i-1][0])/25)
                        ty = round((liste_coords[i-1][1] - liste_coords[i][1])/25)
                        for j in range(tx+1):
                            grille[y-j][x+j] = lettre
                        x = x + tx
                        y = y + ty
                    else: #x> et y<
                        tx = round((liste_coords[i][0] - liste_coords[i-1][0])/25)
                        ty = round((liste_coords[i-1][1] - liste_coords[i][1])/25)
                        for j in range(tx+1):
                            grille[y+j][x+j] = lettre
                        x = x + tx
                        y = y + ty
                else: #on baisse les indices
                    if liste_coords[i][1] > liste_coords[i-1][1]: #x< et y>
                        tx = round((liste_coords[i][0] - liste_coords[i-1][0])/25)
                        ty = round((liste_coords[i-1][1] - liste_coords[i][1])/25)
                        for j in range(tx,0):
                            grille[y+j][x+j] = lettre
                        x = x + tx
                        y = y + ty
                    else: #x< et y<
                        tx = round((liste_coords[i][0] - liste_coords[i-1][0])/25)
                        ty = round((liste_coords[i-1][1] - liste_coords[i][1])/25)
                        for j in range(tx,0):
                            grille[y-j][x+j] = lettre
                        x = x + tx
                        y = y + ty
                    
#contours_cible
contours(liste_contours_coords, grille, x_c, y_c, 'C')
                
#meme methode pour les contours du terrain d'evolution

x_te = round((liste_terrain_coords[0][0] - min_x)//25)
y_te = round((max_y - liste_terrain_coords[0][1])//25)
grille[y_te][x_te] = 'T'

contours(liste_terrain_coords, grille, x_te, y_te, 'T')

#contours des polygones

poly = tuple({} for m in range(len(liste_polygones_coords))) #pour pouvoir iterer sur les indices

for m in range(len(liste_polygones_coords)):
    poly[m]["x_p"] = round((liste_polygones_coords[m][0][0] - min_x)/25)
    poly[m]["y_p"] = round((max_y - liste_polygones_coords[m][0][1])/25)
    grille[poly[m]["y_p"]][poly[m]["x_p"]] = 'P'
    contours(liste_polygones_coords[m], grille,poly[m]["x_p"],poly[m]["y_p"], 'P' )



#contours des cercles
cer = tuple({} for m in range(len(liste_cercles_coords))) #pour pouvoir iterer sur les indices

for m in range(len(liste_cercles_coords)):
    rayon = liste_cercles_coords[m][1]
    rayon_carreau = rayon//25
    
    cer[m]["x_o"] = round((liste_cercles_coords[m][0][0] - min_x)/25)
    cer[m]["y_o"] = round((max_y - liste_cercles_coords[m][0][1])/25)
    #on modelise le cercle par un carre donc on fait tout le tour y compris
    #les diagonales
    liste_diag_cercles = [[liste_cercles_coords[m][0][0]+rayon,liste_cercles_coords[m][0][1]-rayon],[liste_cercles_coords[m][0][0]+rayon,liste_cercles_coords[m][0][1]+rayon],[liste_cercles_coords[m][0][0]-rayon,liste_cercles_coords[m][0][1]+rayon],[liste_cercles_coords[m][0][0]-rayon,liste_cercles_coords[m][0][1]-rayon]]
    grille[cer[m]["y_o"]+rayon_carreau][cer[m]["x_o"]+rayon_carreau] = 'O'
    contours(liste_diag_cercles, grille, cer[m]["x_o"] + rayon_carreau, cer[m]["y_o"] + rayon_carreau, 'O')

for i in range(len(grille)):
    print(grille[i])
    
#on cherche maintenant a calculer les trajectoires

#on initialise la position du robot dans la grille
pos_robot = [round((pos_depart_coords[0][0] - min_x)/25),round((max_y - pos_depart_coords[0][1])/25)]
#on fait la liste des points de passage, avec d'abord les étapes puis le point de tir et l'arrivee en dernier
pts_passage = []
for i in range(len(liste_etapes_coords)):
    pts_passage.append([round((liste_etapes_coords[i][0][0] - min_x)/25),round((max_y - liste_etapes_coords[i][0][1])/25)])
pts_passage.append([round((pos_tir_cible_coords[0][0] - min_x)/25),round((max_y - pos_tir_cible_coords[0][1])/25)])
pts_passage.append([round((pos_arrivee_coords[0][0] - min_x)/25),round((max_y - pos_arrivee_coords[0][1])/25)])







 

#dist_x = abs(pos_robot[0]-pts_passage[0][0])
#dist_y = abs(pos_robot[1]-pts_passage[0][1])
#if dist_x >= dist_y:
#    
#for i in range(len(pts_passage)-1):
#    dist_x = abs(pts_passage[i][0] - pts_passage[i+1][0])
#    dist_y = abs(pts_passage[i][1] - pts_passage[i+1][1])
#    if dist_x >= dist_y:
        



#print(data_env["obstacles"][1])
#dataframe_env = json_normalize(data_env)    #Création du dataframe concernant toutes les images pour ensuite pouvoir afficher les données
#dataframe_path = json_normalize(data_path)
#
#
#grouped = dataframe_env.groupby('terrain_evolution').count()
##print(grouped)
#print(dataframe_env)