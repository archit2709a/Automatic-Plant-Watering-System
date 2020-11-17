import serial
from airtable import Airtable
import time
from datetime import datetime
flow = False

ser = serial.Serial('/dev/ttyACM0', 9600)
now = time.time()

while True:
    if (ser.in_waiting > 0):
        line = (str(ser.readline()))
        strline = line.replace("b'"," ")[:-5]
        if '!' in strline:
            strline = strline.split('!')[0]
        print(strline)
        if (time.time() - now > 10):
            airtable = Airtable('app42vXuBNsAVyqTG', 'Test', 'keyVVJGhlDj5vfPCd')
            if 'Moisture Percentage =' in strline:
                Value = strline.replace('Moisture Percentage = ', ' ')
                Time = datetime.now()
                current_time = datetime.now().strftime("%H:%M:%S")
                records = [{'Value': Value, 'Time': current_time}]
                airtable.batch_insert(records)
                now = time.time()
        if 'ON' in strline:
            flow_time = time.time()
            print("water")
            airtable = Airtable('app42vXuBNsAVyqTG', 'Water', 'keyVVJGhlDj5vfPCd')
            water_time = datetime.now().strftime("%H:%M:%S")
            flow = True
        elif flow == True:
            if 'Flow= ' in strline:
                print("flow")
                Flow = strline.replace("Flow= ", " ")
                flow = False
        elif 'OFF' in strline:
            print('off')
            end_time = time.time()
            airtable = Airtable('app42vXuBNsAVyqTG', 'Water', 'keyVVJGhlDj5vfPCd')
            end_time = datetime.now().strftime("%H:%M:%S")
            records = [{'Time': water_time, 'Flow': Flow, 'Endtime': end_time}]
            airtable.batch_insert(records)
            
            
            