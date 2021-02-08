import serial
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import networkx as nx
import random
import math

# load edge list from sample file
fh = open("sample_net2.txt", "rb")
G = nx.read_edgelist(fh) # define graph object
fh.close()

# initialize figure
plt.ion()
fig=plt.figure()

# open serial com
ser = serial.Serial('COM5',9600)
ser.close()
ser.open()

# make dictionary of node degrees
degree_dict = dict(G.degree(G.nodes()))
nx.set_node_attributes(G, degree_dict,'degree')

# make dictionary for 'group' attribute'
group_dict = dict()
for n in G.nodes():
    group_dict[n] = random.randint(1,2)

# map group attribute to node color
color_dict = dict()
for n in group_dict:
    if group_dict[n] == 1:
        color_dict[n] = "blue"
    else:
        color_dict[n] = "red"


# steps between degrees
steps = 2

# real time plotting
while True:

    # read and split incoming data
    data = ser.readline()
    data_s = data.decode()
    data_f = data_s.split(' ')

    # nodes to include based on 'group'
    if data_f[1] == 'L':
        G2 = nx.subgraph(G,[x for x in G.nodes() if group_dict[x]==2])
    elif data_f[2][0] == 'L':
        G2 = nx.subgraph(G,[x for x in G.nodes() if group_dict[x]==1])
    elif data_f[1] == "H" and data_f[2][0] == "H":
        G2 = G
    else:
        G2 = G

    # nodes to include based on degree
    if float(data_f[0]) < 11:
        num = float(data_f[0])
        G3 = nx.subgraph(G2,[x for x in G2.nodes() if degree_dict[x] > num])
        txt = 'degree > ' + str(num)
    else:
        G3 = G2
         
    plt.clf()
    
    # construct color map for included nodes
    color = []
    for x in G3.nodes():
        color.append(color_dict[x])

    # re-draw the network
    nx.draw_shell(G3, node_color = color, with_labels = False)
    plt.text(0.8, 1,txt,
     horizontalalignment='center',
     verticalalignment='center')

    plt.show()
    plt.pause(0.00001)  


