#!/bin/bash
DIR="~/Images"
GPIO=26
TRUE=1
FALSE=0
etatGPIO=0
cpt=0
#verification acces root
if [ $EUID != 0 ]
then
	echo Il faut etre root pour executer le script. Essaye sudo $0
	exit
fi


echo debut du script
echo ctrl-c pour en sortir
echo
#setup pin et direction
echo $GPIO > /sys/class/gpio/export
echo in /sys/class/gpio/gpio$GPIO/direction
#trap &quot;cleanup $GPIO&quot; SIGHUP SIGINT SIGTERM

while true
do
	lecture1=$(cat /sys/class/gpio/gpio$GPIO/value)
	if [[ $lecture1 == $etatGPIO ]]
	then
		echo etat bas recu
		echo
		echo declancement de la commande ...
		echo
		fswebcam -p YUYV -d /dev/video0 /media/Images/$(date +"%Y-%m-%d_%H%M")_$cpt.jpg
		cpt=$((cpt+1))
#		sleep 2
	fi

done

