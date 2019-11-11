from collections import deque
from Node import *
import time

class Graph:

    MAX_LENGTH = 2**16
    clients = 0
    providers = 0
    peers = 0

    def __init__(self):
        # self.data = [Node() for i in range(Graph.MAX_LENGTH)] 
        self.data = [None] * Graph.MAX_LENGTH
        self.total_nodes = 0

    def reset_stats(self):
        Graph.clients = Graph.providers = Graph.peers = 0
    
    def insertEdge(self, tail, head, relation):

        tail = int(tail)
        head = int(head)

        if self.data[tail] is None:
            self.total_nodes += 1
            self.data[tail] = Node()
        if self.data[head] is None:
            self.total_nodes += 1
            self.data[head] = Node()

        self.data[tail].id = tail
        self.data[tail].edges.append(type('',(object,),{'relationship':int(relation),'head':self.data[head], 'headid':head})())

    def initGraph(self, filename):
        """Opens the input file, reads it and creates the tree based on the information contained"""

        f = open(filename)
        lines = f.read().splitlines()
        for x in lines:
            #TODO: check input
            # if not checkPrefix(x.split()[0]):
            #     f.close()
            #     raise Exception
            self.insertEdge(*x.split())
        f.close()

    def Dijkstra(self):
        i = 0
        t0 = time.time()
        for node in self.data:
            if node is not None:
                # print(i)
                node.dijkstra(3, i)
                # print()
            if i % 500 == 0:
                print(i, round((time.time() - t0), 2))
                t0 = time.time()
            i += 1

        print("Elapsed time:", time.time() - t0)       
        print("Stats:")
        total_connections = self.total_nodes*(self.total_nodes - 1)
        Graph.providers = total_connections - Graph.peers - Graph.clients
        print("Providers:", Graph.providers/total_connections*100)
        print("Peers:", Graph.peers/total_connections)
        print("Clients:", Graph.clients/total_connections)