
# Team Id: 

# Author List: Pankaj Kumar (173050052) , Avais Ahmad (173050043)

# Filename: console.py

# Theme: showing the real time movements of the joystick.

# Functions:  make_fig1, Serial, drawnow

# Global Variables: xNum , yNum




import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as anim
from drawnow import *
import numpy as np
import pandas as pd


# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/COM3',
    baudrate=115200,
    #parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

ser.isOpen()


# tell matplotlib you want interactive mode to plot data
#plt.ion()


#global variables
xNum=0
yNum=0

def make_fig1():
    plt.axis([0, 40, 0, 40])
    plt.ylim(0,40)                                 #Set y min and max values
    plt.xlim(0,40)                                 #Set y min and max values
    plt.title('Joystick Movement')                  #Plot the title
    plt.grid(True)                                  #Turn the grid on
    plt.plot(40-xNum, yNum,'-ro' )


drawnow(make_fig1)

x=""
y=""
while 1:
    re=(ser.read(1)).decode("utf-8")
    
    if(re == 'X'):
            re=(ser.read(1)).decode("utf-8")
            if(re==':'):
                re=(ser.read(1)).decode("utf-8")
                while (re  != chr(13)):
                    x += re
                    re=ser.read(1).decode("utf-8")

    if(re == 'Y'):
            re=(ser.read(1)).decode("utf-8")
            if(re==':'):
                re=(ser.read(1)).decode("utf-8")
                while (re  != chr(13)):
                    y += re
                    re=ser.read(1).decode("utf-8")
    
    if(x != ''):
        if(y != ''):
            xNum = float(x) # turn string into numbers
            yNum = float(y) # turn string into numbers
            x=""
            y=""
            
    print(xNum)
    print(yNum)    
    drawnow(make_fig1)

   
