import socket
from threading import Thread
from time import sleep

import numpy as np
import pandas as pd
from keras.models import Sequential
from keras.layers import Dense
from keras.models import load_model

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

def resample(listIN, numSAMP):
    numIN = len(listIN)
    factor = numIN / numSAMP
    listOUT = []
    for i in range(numSAMP-1):
        if i==0:
            listOUT.append(float(listIN[i]))#left
        numDown = int( i*factor )
        val = ( float(listIN[numDown])+float(listIN[numDown]))/2
        listOUT.append(val)

    return listOUT
        
        

numberOfData = 100
count = 0
if __name__ == '__main__':

    #load model
    model = load_model("Spell_model.h5")
    classes = ['ALOHOMORA', 'APARECUM', 'AQUAMENTI', 'ARRESTO MOMENTO', 'ASCENDIO', 'DESCENDIO', 'EXPELLIARMUS', 'FINITE INCANTATEM', 'FLABILIS', 'HERBIVICUS', 'INCENDIO', 'LOCOMOTOR', 'LUMOS', 'METEOLOJINX', 'MIMBLEWIMBLE', 'NOX', 'OPPUGNO', 'REPARO', 'REVELIO', 'SILENCIO', 'SPECIALIS REVELIO', 'STUPIFY', 'TARANTALLEGRA', 'WINGARDIUM LEVIOSA']
        
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
                xyz = resample(xlist, numberOfData) + resample(ylist, numberOfData) + resample(zlist, numberOfData)

                a = np.array(xyz)
                a = a.reshape(1,300)
                #print(a.shape)
                predict = model.predict(a)
                predict = max(predict)
                #print(predict)
                list_predict = predict.tolist()
                index_pred = list_predict.index(max(list_predict))
                print(classes[index_pred])
            else:
                continue
            

                

