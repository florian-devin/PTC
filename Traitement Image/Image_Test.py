#Importations des librairies
import numpy as np 
import cv2 
import matplotlib.pyplot as plt 
from PIL.Image import * 


#Lecture de l'image
Chemin="Obstacles/Coeur_Violet.png"

Image_Couleur=open(Chemin)
Image_Forme= cv2.imread(Chemin) 



#Image en Gris 
Image_Gris = cv2.cvtColor(Image_Forme, cv2.COLOR_BGR2GRAY)

#Sueillage
ret,Image_Seuillee= cv2.threshold(Image_Gris, 0 , 255, cv2.THRESH_BINARY)
#plt.imsave('Image_Test/Image_Seuille.png', Image_Seuillee , cmap=plt.cm.gray)

###################### Detection Couleur #############################

#Recupérer valeur d'un pixel (place 10,10) 
c=Image.getpixel(Image_Couleur,(10,10))
#print (c)

#fonction pour detecter couleur 
def Detecter_Couleur(c):
    if c[0] >= 150 and c[1] <= 20 and c[2] <= 80:
        print ("La Couleur est Magenta")
    elif c[0] <= 150 and  c[1] >= 150 and c[2] >= 150 :
        print ("La Couleur est Cyan")
    elif c[0] >= 150 and  c[1] >= 150 and c[2] <= 150 :
        print ("La Couleur est Jaune")
    elif c[0] >= 80 and  c[1] <= 80 and c[2] >= 80 :
        print ("La Couleur est Violet")
    elif c[0] <= 150 and  c[1] >= 150 and c[2] <= 150 :
        print ("La Couleur est Verte")
    elif c[0] >= 200 and  c[1] >= 100 and c[2] <= 20 :
        print ("La Couleur est Orange")
    else:
        print ("None")
        

###################### Detection Forme ###############################

#Contours 

#Filtre de Canny 


Image_Canny=cv2.Canny(Image_Gris, 0 , 255)
#plt.imsave('Image_Test/Image_Canny.png', Image_Canny , cmap=plt.cm.gray)

contours,resh=cv2.findContours(Image_Canny,cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
NombredeFormes=resh
contours=contours[0]


approx = cv2.approxPolyDP(contours,0.01*cv2.arcLength(contours,True),True)
print(approx)
print ('le nombre de contours est ',len(approx)) 
if len(approx)==4 and NombredeFormes<2: #si une forme détectée et si 4 cotés
    print("La forme est Carrée") 
if len(approx)==4 and NombredeFormes>=2: #si deux formes détéctées si 4 cotés
    print("La forme est un Carré Creux")
if len(approx)==3:
    print("La forme est Triangulaire") #si une forme détéctée et 3 cotés
if len(approx)==12:
    print("La forme est une Croix")#si 12 cotés et 1 forme détectée 
if len(approx)>=14:
    print("La forme est Circulaire")#si beaucoup de cotés (+ de 14) et une forme 
if len(approx)==13:
    print("La forme est un coeur")# si beaucoup de cotés mais moins que cercle 
    # attention à pas confondre avec une croix ... 


   
    ###################### Affichage #############################

Detecter_Couleur(c)

#Dimensions de l'image
[n,m]= Image_Couleur.size
print("Les dimensions de l'image sont",[n,m])


