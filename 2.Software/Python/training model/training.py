import pandas as pd
import numpy as np
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import StratifiedShuffleSplit
from keras.models import Sequential
from keras.layers import Dense
from keras_visualizer import visualizer 
from keras.utils import np_utils


data = pd.read_csv('data.csv')#导入训练集

def segment(trainData):

    label_encoder = LabelEncoder().fit(trainData.clf)
    labels = label_encoder.transform(trainData.clf)
    classes = list(label_encoder.classes_)
    train = trainData.drop(['clf'], axis=1)
    
    return train, labels, classes

train, labels, classes = segment(data)
train = train.values
#print(train)
sss = StratifiedShuffleSplit(train_size=0.8,test_size=0.2, random_state=0)

#分割数据集
for train_index, valid_index in sss.split(train, labels):
    x_train, x_valid = train[train_index], train[valid_index]
    y_train, y_valid = labels[train_index], labels[valid_index]

#x_train = (x_train-np.mean(x_train))/np.var(x_train)
#x_valid = (x_valid-np.mean(x_valid))/np.var(x_valid)

#训练参数
nb_features = 100    
nb_class = len(classes)
num_pixels = nb_features*3
num_classes = nb_class
y_train = np_utils.to_categorical(y_train, nb_class)
y_valid = np_utils.to_categorical(y_valid, nb_class)

#训练模型
model = Sequential()
model.add(Dense(num_pixels, input_dim=num_pixels, kernel_initializer='normal', activation='relu'))
model.add(Dense(num_classes, kernel_initializer='normal', activation='softmax'))
# Compile model
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

model.fit(x_train, y_train, validation_data=(x_valid, y_valid), epochs=100, verbose=2)
visualizer (model)
model.save('model.h5')
model.summary()

