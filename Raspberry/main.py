import os
import cv2
import serial
import signal
from time import sleep

#--------------------------
#Parametre pour les prises de vue
nb_photos = 1
continue_photos = False
time_between_photos = 1
#--------------------------
img_counter = 1



uart = serial.Serial("/dev/ttyS0", 19200)
cam  = cv2.VideoCapture(0)

def takePicture():
    global img_counter
    ret, frame = cam.read()
    img_name = "/media/PTC/Images/img{}.png".format(img_counter)
    cv2.imwrite(img_name, frame)
    print("img{}.png is written !".format(img_counter))
    img_counter += 1

#Attente de l'initialisation de l'esclave
while (uart.read() != b'a'):
    continue
uart.write(b'b')

#Programme principale
while True:
    data = uart.read_until(b'\r').decode()
    data_list = data.split(' ')
    if data_list[0] == "PPH":
        for parameter in data_list[1:]:
            if parameter[0] == "O":
                #prendre une unique photo
                continue_photos = False
            elif parameter[0] == "C":
                #prendre des photos en continue
                continue_photos = True
            elif parameter[0] == "S":
                #nombre determiner de photo
                continue_photos = False
            elif parameter[0] == "E":
                #duree entre deux photo (en 100ms)
                time_between_photos = float(parameter.split(":")[1])
            elif parameter[0] == "N":
                #nombre de photo a prendre
                nb_photos = int(parameter.split(":")[1])

        if continue_photos == True :
            pid = os.fork()
            if pid == 0: #process enfant
                while continue_photos :
                    takePicture()
                    sleep(time_between_photos/10)
            else :
                data = uart.read_until(b'\r').decode()
                if (data[0:3] == "SPH"):
                    os.kill(pid, signal.SIGKILL) #kill child process
                    continue_photos = False

        else:
            #prise de photo pas en continue
            for i in range(0,nb_photos):
                takePicture()
                sleep(time_between_photos/10)
            nb_photos = 1




