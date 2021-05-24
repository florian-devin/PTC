# -*- coding: utf-8 -*-
"""
Created on Tue May 11 08:42:40 2021

@author: Thomas
"""

import serial
import time
import PySimpleGUI as sg

ser1 = serial.Serial()
ser1.port = 'COM3'
ser1.baudrate = 19200
ser1.bytesize = 8
ser1.parity = 'N'
ser1.stopbits = 1


ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 19200
ser.bytesize = 8
ser.parity = 'N'
ser.stopbits = 1


def envoi_message(msg):
    msg = msg + '\r'
    ser.open()
    ser.write(msg.encode('ascii'))
    print(msg.encode('ascii'))
    print(msg)
    ser.close()

def lire_message():
    ser1.open()
    com = ser1.read(ser1.inWaiting()).decode('ascii')
    ser1.close()
    return com
    
    


## creation de la fenêtre

layout = [[sg.Cancel(),sg.Button("Début de l'epreuve", key = '-START-'),sg.Text('type_epreuve(1-8) :'),sg.InputText('',size = (1,1)),sg.Button("Fin de l'epreuve", key = '-END-'), sg.Button("Arret d'urgence", key = '-USTOP-')],
            [sg.Button("Avancer", key = '-FORWARD-'), sg.Text('Avancer à la vitesse(5-100) :'),sg.InputText('', size = (3,1)),sg.Button("Reculer", key = '-BACKWARD-'), sg.Text('Reculer à la vitesse(5-100) :'),sg.InputText('', size = (3,1)),sg.Button("Regler la vitesse", key = '-SPEED-'), sg.Text('Mettre la vitesse a(5-100) :'),sg.InputText('20', size = (3,1)), sg.Button("Stop", key = '-STOP-'),],
            [sg.Button("Rotation 90 à droite", 'key = -RD-'),sg.Button("Rotation 90 à gauche", 'key = -RG-'),sg.Button("Rotation 180", key = '-RC-'), sg.Text('Tourner de 180 vers (D/G) :'), sg.InputText('', size = (1,1)), sg.Button("Rotation spécifiée", key = '-RA-'),sg.Text('Rotation de (D/G):(0-180)'),sg.InputText('D',size = (1,1)),sg.InputText('90', size = (3,1))],
            [sg.Button("Mesure courant", key = '-MI-'),sg.Button("Detection d'obstacle", key = '-OBS-'), sg.Text('Parametre D pour avant et arriere :'),sg.InputText('', size = (1,1))],
            [sg.Button("Allumage pointeur lumineux", key = '-LON-'), sg.Text('Intensité(1-100), Durée allumage(0,1-9,9), Durée extinction(0,1-9,9), Nombre d''allumages(1-99)'),sg.InputText('',size = (3,1)),sg.InputText('',size = (3,1)),sg.InputText('',size = (3,1)),sg.InputText('',size = (3,1)),sg.Button("Fin allumage", key = '-LOFF-')],
            [sg.Button("Commande servomoteur", key = '-SERVO-'), sg.Text('Sens(H/V) et angle(-90 - 90))'), sg.InputText('', size = (1,1)), sg.InputText('', size = (3,1)), sg.Button("Prise de photos", key = '-PIC-'), sg.Text('Mode(O/C/S), Temps entre 2 photos(0-9,9), Nb de photos(1-255)'),sg.InputText('', size = (3,1)),sg.InputText('', size = (3,1)),sg.InputText('', size = (3,1)),sg.Button("Arret photo", key = '-PICOFF-')]]


window = sg.Window('Centrale de commande',layout)
while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED or event == 'Cancel': # if user closes window or clicks cancel
        break
    elif event == '-START-':
        msg = 'D'
        if values[0] != '':
            msg = 'D' + ' ' + values[0]
    elif event == '-END-':
        msg = 'E'
    elif event == '-USTOP-':
        msg = 'Q'
    elif event == '-FORWARD-':
        msg = 'A'
        if values[1] != '':
            msg = 'A' + ' ' + values[1]
    elif event == '-BACKWARD-':
        msg = 'B'
        if values[2] != '':
            msg = 'B' + ' ' + values[2]
    elif event == '-SPEED-':
        msg = 'TV' + ' ' + values[3]
    elif event == '-STOP-':
        msg = 'S'
    elif event == '-RD-':
        msg = 'RD'
    elif event == '-RG-':
        msg = 'RG' 
    elif event == '-RC-':
        msg = 'RC'
        if values[2] != '':
            msg = 'RC' + ' ' + values[4]
    elif event == '-RA-':
        msg = 'RA' + ' ' + values[5] + ':' + values[6]
    elif event == '-MI-':
        msg = 'MI'
    elif event == '-OBS-':
        msg = 'MOU'
        if values[7] != '':
            msg = 'MOU' + ' ' + values[7]
    elif event == '-LON-':
        msg = 'L'
        if values[8] != '' and values[9] != '' and values[10] != '' and values[11] != '':
            msg = 'L' + ' ' + 'I:' + values[8] + ' ' + 'D:' + values[9] + ' ' + 'E:' + values[10] + ' ' + 'N:' + values[11]
        elif values[8] == '' and values[9] != '' and values[10] != '' and values[11] != '':
            msg = 'L' + ' ' + 'D:' + values[9] + ' ' + 'E:' + values[10] + ' ' + 'N:' + values[11]
        elif values[8] != '' and values[9] == '' and values[10] != '' and values[11] != '':
            msg = 'L' + ' ' + 'I:' + values[8] + ' ' + 'E:' + values[10] + ' ' + 'N:' + values[11]
        elif values[8] != '' and values[9] != '' and values[10] == '' and values[11] != '':
            msg = 'L' + ' ' + 'I:' + values[8] + ' ' + 'D:' + values[9] + ' ' + 'N:' + values[11]
        elif values[8] != '' and values[9] != '' and values[10] != '' and values[11] == '':
            msg = 'L' + ' ' + 'I:' + values[8] + ' ' + 'D:' + values[9] + ' ' + 'E:' + values[10]
        elif values[8] == '' and values[9] == '' and values[10] != '' and values[11] != '':
            msg = 'L' + ' ' + 'E:' + values[10] + ' ' + 'N:' + values[11]
        elif values[8] == '' and values[9] != '' and values[10] == '' and values[11] != '':
            msg = 'L' + ' ' + 'D:' + values[9] + ' ' + 'N:' + values[11]
        elif values[8] == '' and values[9] != '' and values[10] != '' and values[11] == '':
            msg = 'L' + ' ' + 'D:' + values[9] + ' ' + 'E:' + values[10]
        elif values[8] != '' and values[9] == '' and values[10] == '' and values[11] != '':
            msg = 'L' + ' ' + 'I:' + values[8] + ' ' + 'N:' + values[11]
        elif values[8] != '' and values[9] == '' and values[10] != '' and values[11] == '':
            msg = 'L' + ' ' + 'I:' + values[8] + ' ' + 'E:' + values[10]
        elif values[8] != '' and values[9] != '' and values[10] == '' and values[11] == '':
            msg = 'L' + ' ' + 'I:' + values[8] + ' ' + 'D:' + values[9]
        elif values[8] != '':
            msg = 'L' + ' ' + 'I:' + values[8]
        elif values[9] != '':
            msg = 'L' + ' ' + 'D:' + values[9]
        elif values[10] != '':
            msg = 'L' + ' ' + 'E:' + values[10]
        else:
            msg = 'L' + ' ' + 'N:' + values[11]
    elif event == '-LOFF-':
        msg = 'LS'
    elif event == '-SERVO-':
        msg = 'CS'
        if values[12] != '':
            msg = 'CS' + ' ' + values[12]
        elif values[13] != '':
            msg = 'CS' + 'A:' + values[13]
    elif event == '-PIC-':
        msg = 'PPH'
        if values[14] != '' and values[15] != '' and values[16] != '':
            msg = 'PPH' + ' ' + values[14] + ' ' + 'E:' + values[15] + ' ' + 'N:' + values[16]
        elif values[14] == '' and values[15] != '' and values[16] != '':
            msg = 'PPH' + ' ' + 'E:' + values[15] + ' ' + 'N:' + values[16]
        elif values[14] != '' and values[15] == '' and values[16] != '':
            msg = 'PPH' + ' ' + values[14] + ' ' + 'N:' + values[16]
        elif values[14] != '' and values[15] != '' and values[16] == '':
            msg = 'PPH' + ' ' + values[14] + ' ' + 'E:' + values[15]
        elif values[14] != '':
            msg = 'PPH' + ' ' + values[14]
        elif values[15] != '':
            msg = 'PPH' + ' ' + 'E:' + values[15]
        else:
            msg = 'PPH' + ' ' + 'N:' + values[16]
    elif event == '-PICOFF-':
        msg = 'SPH'
    
    
    envoi_message(msg)
    print(lire_message())

        
        
window.close()
    