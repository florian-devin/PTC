# -*- coding: utf-8 -*-
"""
Created on Tue May 11 08:42:40 2021

@author: Thomas
"""

import serial
import time
import PySimpleGUI as sg

ser = serial.Serial()
ser.port = 'COM4'
ser.baudrate = 19200
ser.bytesize = 8
ser.parity = 'N'
ser.stopbits = 1

ser1 = serial.Serial()
ser1.port = 'COM2'
ser1.baudrate = 19200
ser1.bytesize = 8
ser1.parity = 'N'
ser1.stopbits = 1



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
    
    
#sg.theme('DarkAmber')   # Add a touch of color
## All the stuff inside your window.
#layout = [  [sg.Text('Some text on Row 1')],
#            [sg.Text('Enter something on Row 2'), sg.InputText()],
#            [sg.Button('Ok'), sg.Button('Cancel')] ]
#
## Create the Window
#window = sg.Window('Window Title', layout)
## Event Loop to process "events" and get the "values" of the inputs
#while True:
#    event, values = window.read()
#    if event == sg.WIN_CLOSED or event == 'Cancel': # if user closes window or clicks cancel
#        break
#    print('You entered ', values[0])
##
#window.close()
#
#event, values = sg.Window('Get filename example', [[sg.Text('Filename')], [sg.Input(), sg.FileBrowse()], [sg.OK(), sg.Cancel()] ]).read(close=True)

#layout = [[sg.InputText('')],
#        [sg.Button('Ok'), sg.Button('Cancel'),sg.Submit('Envoyer')]]
#window = sg.Window('Window Title', layout)
#while True:
#    event, values = window.read()
#    if event == sg.WIN_CLOSED or event == 'Cancel': # if user closes window or clicks cancel
#        break
#window.close()
#
#gui_rows = [[sg.Text('Robotics Remote Control')],
#            [sg.T(' '  * 10), sg.RealtimeButton('Forward')],
#            [sg.RealtimeButton('Left'), sg.T(' '  * 15), sg.RealtimeButton('Right')],
#            [sg.T(' '  * 10), sg.RealtimeButton('Reverse')],
#            [sg.T('')],
#            [sg.Quit(button_color=('black', 'orange'))]
#            ]
#
#window = sg.Window('Robotics Remote Control', gui_rows)
#
##
## Some place later in your code...
## You need to perform a Read or Refresh call on your window every now and then or
## else it will apprear as if the program has locked up.
##
## your program's main loop
#while (True):
#    # This is the code that reads and updates your window
#    event, values = window.read(timeout=50)
#    print(event)
#    if event in ('Quit', sg.WIN_CLOSED):
#        break
#
#window.close()  # Don't forget to close your window!
#
#
#layout = [ [sg.Text('My layout', key='-TEXT-')],
#           [sg.Button('Read')]]
#
#window = sg.Window('My new window', layout)
#
#while True:             # Event Loop
#    event, values = window.read()
#    if event == sg.WIN_CLOSED:
#        break
#    window['-TEXT-'].update('My new text value')
#    
#def test_button():
#    print("oui")
#layout = [[sg.InputText('')],
#        [sg.Button('Ok'), sg.Button('Cancel'),sg.Submit('Envoyer'),sg.Button('test', key = 'test_button',)]]
#window = sg.Window('Window Title', layout)
#while True:
#    event, values = window.read()
#    if event == sg.WIN_CLOSED or event == 'Cancel': # if user closes window or clicks cancel
#        break
#    elif event == 'test_button':
#        test_button()
#window.close()


## creation de la fenêtre

layout = [[sg.Cancel(),sg.Button("Début de l'epreuve", key = '-START-'),sg.Text('type_epreuve(1-8) :'),sg.InputText('',size = (1,1)),sg.Button("Fin de l'epreuve", key = '-END-'), sg.Button("Arret d'urgence", key = '-USTOP-')],
            [sg.Button("Avancer", key = '-FORWARD-'), sg.Text('Avancer à la vitesse(5-100) :'),sg.InputText('', size = (3,1)),sg.Button("Reculer", key = '-BACKWARD-'), sg.Text('Reculer à la vitesse(5-100) :'),sg.InputText('', size = (3,1)),sg.Button("Regler la vitesse", key = '-SPEED-'), sg.Text('Mettre la vitesse a(5-100) :'),sg.InputText('20', size = (3,1)), sg.Button("Stop", key = '-STOP-')]]


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
    envoi_message(msg)
    print(lire_message())

        
        
window.close()
    