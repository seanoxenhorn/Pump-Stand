# I am really hoping this DAQ works, come bakc and let me know if we were successful - Sean Oxenhorn, 10/7/2024
# Cal Poly Space Systems Liquids Data Acquisiton Code

import numpy as np
import serial
import schedule
import time


def main_func():
   
    arduino_data = arduino.readline()

    decoded_values = str(arduino_data[0:len(arduino_data)].decode("utf-8"))
    list_values = decoded_values.split('x')

    PT1 = [] #Init Pressure Transducer before pump
    PT2 = [] #Init Pressure Transducer after pump
    FM = [] #Init Flow Meter
    t = [] # Init time stamp
    for i in range(len(0, list_values)):
        
        if i % 3 == 0:  # If index is 1, 4, 7...
            PT1.append([list_values[i]])  # Append to Pressure Transducer 2
        elif i % 3 == 1: # If index is 2, 5, 8...
            PT2.append([list_values[i]])  # Append to Pressure Transducer 2
        else: #If index is 3, 6, 9...
            FM.append([list_values[i]])  # Append to Flow Meter Matrix

    arduino_data = 0
    list_values.clear() # will have to debug to make sure all the data keeps filing into each matrix properly if we keep iterating it to read arduino lines
    return PT1, PT2, FM, t

def calcs():
    
    # Calculate pressure transducer voltage to psia
    # Calculate press reg data
    # Calculate thermocouple data to temp F??? maybe K
    # Calculate flow meter data
    # Calculate Load cell data
    # FIGURE OUT GUI SO WE CAN CLICK ON WHAT VALVES TO ACTUATE


    # Graph data against time

# ----------------------------------------Main Code------------------------------------
# Declare variables to be used
    list_values = []

arduino = serial.Serial('com3', 9600) #Connect to Arduino via COM
print('Established serial connection to Arduino')


print('Program started')

# Setting up the Arduino
schedule.every(0.1).seconds.do(main_func)

while True:
    schedule.run_pending()
    time.sleep(0.01)