from collections import deque
from Node import *

class Graph:
    def __init__(self):
        self.data = [None] * 2**16
    
    def insertNode(self, tail, head, type):
        tail = int(tail)
        if self.data[tail] is None:
            self.data[tail].list = []
            self.data[tail].visited = False
        try:
            headNode = self.data[int(head)]
        self.data[tail].list.append(Node(headNode, type))

    def initGraph(self, filename):
        """Opens the input file, reads it and creates the tree based on the information contained"""

        f = open(filename)
        lines = f.read().splitlines()
        for x in lines:
            #TODO: check input
            # if not checkPrefix(x.split()[0]):
            #     f.close()
            #     raise Exception
            self.insertNode(*x.split())
        f.close()

