import socket
from threading import Thread
from time import sleep

import numpy as np
import pandas as pd

## recive data from UDP
def recv_data():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #addr = ("192.168.12.99",2333)
    addr = ("192.168.1.100",23333)
    s.bind(addr)
    recvdata, client_address = s.recvfrom(1024)
    recvdata = recvdata.decode('utf-8')
    data = recvdata.split(",")
    #print("[ %s ] from:[ %s ]\n"%(recvdata,client_address))
    #print(data)
    s.close()
    return data

## resample to numSAMP
def resample(listIN, numSAMP):
    numIN = len(listIN)
    factor = numIN / numSAMP
    listOUT = []
    for i in range(numSAMP-1):
        if i==0:
            listOUT.append(float(listIN[i]))#left
        numDown = int( i*factor )
        val = ( float(listIN[numDown])+float(listIN[numDown]))/2
        listOUT.append(str(val))

    return listOUT
        
        

numberOfData = 100
count = 0
if __name__ == '__main__':
    while True:
        data = recv_data()
        
        #no output if buttom unpressed
        if data == ['0']:
            #print('no data')
            continue

        #output list of data if buttom pressed 
        else :
            xlist = []
            ylist = []
            zlist = []
            while data!=['0']:
                data = recv_data()
                if data!=['0']:
                    xlist.append(data[0])
                    ylist.append(data[1])
                    zlist.append(data[2])

            if len(xlist)>numberOfData:
                xyz = ['EXPELLIARMUS'] + resample(xlist, numberOfData) + resample(ylist, numberOfData) + resample(zlist, numberOfData)
                print(xyz)
                #print(len(xyz))
                #df = pd.DataFrame([xyz])
                #df.to_csv('data.csv', mode='a', header=None, index=None)

                count = count+1
                print('write finish! %s'%(count))
                
                if count==50:
                    print('50 data! collected ')
            else:
                continue
            

                

