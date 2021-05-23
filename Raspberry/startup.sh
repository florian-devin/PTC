#!/bin/bash
#Fichier de demarage du Raspberry pi 4

#montage de la clef usb
echo "Execution du lancer..."
echo
echo "Si ce programme rencontre des erreurs il faut le lancer avec :

    sudo -E ./startup.sh
    
    "


echo "Les periferiques sont :"
lsblk
echo "

"
name_usb=sda
echo "montage de $name_usb..."
mount /dev/$name_usb /media/
echo "Montage termine

"
echo "/media :"
ls -l -h /media
echo "

"
echo "/media/PTC/Images"
ls -l -h /media/PTC/Images

echo "

"

echo "Lancement du programme main.py"
python3 main.py


