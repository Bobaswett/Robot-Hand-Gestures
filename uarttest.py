import serial
import time
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from sklearn.cluster import AgglomerativeClustering, KMeans
from sklearn.metrics import accuracy_score, confusion_matrix
from sklearn.decomposition import PCA
from PIL import Image
import os
from sklearn.decomposition import PCA
from sklearn.linear_model import LogisticRegression
import cv2
from scipy.stats import mode
from collections import defaultdict

esp = serial.Serial(
port = '/dev/ttyUSB0',
baudrate= 115200,
bytesize = serial.EIGHTBITS,
parity= serial.PARITY_NONE,
stopbits= serial.STOPBITS_ONE,
timeout=5,
xonxoff = False,
rtscts = False,
dsrdtr = False,
writeTimeout = 2
)

def load_images_from_folder(folder):
    images = []
    for filename in os.listdir(folder):
        if filename.endswith(".npy"):
            img = np.load(os.path.join(folder, filename))
            images.append(img)
    return np.array(images)

left_images = load_images_from_folder('/home/jlk/Desktop/pypro/final project/left')
right_images = load_images_from_folder('/home/jlk/Desktop/pypro/final project/right')
back_images = load_images_from_folder('/home/jlk/Desktop/pypro/final project/back')

X = np.concatenate((left_images, right_images, back_images), axis=0)

y = np.array(['left'] * len(left_images) + ['right'] * len(right_images) + ['back'] * len(back_images))


pca = PCA(n_components=2)
X_pca = pca.fit_transform(X)


kmeans = KMeans(n_clusters=3, random_state=42)
kmeans.fit(X_pca)  # X_pca is the 2D PCA-transformed data
cluster_labels = kmeans.labels_


def determine_label_order(y_true,cluster_labels):
    label_order = {}
    for cluster in np.unique(cluster_labels):
        mask = cluster_labels == cluster
        most_common_label = mode(y_true[mask])[0][0]
        label_order[cluster] = most_common_label
    return label_order
label_order = determine_label_order(y,cluster_labels)
print(label_order)

cam = cv2.VideoCapture(0)
width = cam.get(cv2.CAP_PROP_FRAME_WIDTH)
height = cam.get(cv2.CAP_PROP_FRAME_HEIGHT)


d = defaultdict(int)

while True:
    # cam.set(cv2.CAP_PROP_FPS,0.001)
    _, frame = cam.read()
    frameSmall = cv2.resize(frame,(128,128))
    

    img_normalized = frameSmall / 255.0
    img_flattened = img_normalized.flatten()
    
    frame_pca = pca.transform([img_flattened])
    
    direction = kmeans.predict(frame_pca)
    
    print(label_order[direction[0]])

    d[f'{label_order[direction[0]]}'] += 1
    if len(d) > 1:
        d = defaultdict(int)
        d[f'{label_order[direction[0]]}'] += 1

    if list(d.values())[0] > 15:

        direction1 = list(d.keys())[0] 
        print(f'turning {direction1}')
        esp.write(f'{direction1}'.encode())
        d = defaultdict(int)

    cv2.imshow('Frame',frame)
    if cv2.waitKey(1) == ord('q'):
        break
    time.sleep(0.7)
cam.release()
cv2.destroyAllWindows()