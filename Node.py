from collections import deque
import Graph as g

class Node:
    counter = 0

    def __init__(self):
       self.visited = False
       self.edges = []
       self.type = None
       self.hops = 0
       self.id = 0
       self.parent = -1
       Node.counter += 1

    def hasEdges(self):
        return self.edges != []

    def process(self, lists, visited):
        self.visited = not self.visited
        self.type = None
        for edge in self.edges:
            node = edge.head
            if node.visited == visited and node.type is None: 
                node.hops = self.hops + 1
                node.type = 0
                lists[0].append(node)
        
        self.hops = 0


    def commercialDijkstra(self, nlists, visited):
        lists = [deque()] * nlists
        lists[0].append(self)
        for i in range(nlists):
            fifo = lists[i]
            while fifo:
                node = fifo.popleft()
                # breakpoint()
                if node.visited == visited:
                    node.process(lists, visited)
                    

    def dijkstra(self, nlists, visited):
        lists = [deque() for i in range(nlists)]
        lists[nlists-1].append(self)
        for i in range(nlists-1, -1, -1):
            fifo = lists[i]
            while fifo:
                node = fifo.popleft()
                # breakpoint()
                if node.visited == visited:
                    node.process2(lists, visited, i+1)
                                        
    def process2(self, lists, visited, list_num):
        self.visited = not self.visited
        r = 0
        if self.type == 3:
            g.Graph.clients += 1
            # r = "c"
        elif self.type == 2:
            g.Graph.peers += 1
            # r = "p"
        elif self.type == 1:
            g.Graph.providers += 1
            # r = "pv"
        # print(self.id, r, self.parent)
        self.type = None
        for edge in self.edges:
            node = edge.head
            if node.visited == visited:
                if list_num == 3:
                    if (node.type is None or node.type < edge.relationship) and edge.relationship != 1:
                        # breakpoint()
                        node.parent = self.id
                        node.hops = self.hops + 1
                        node.type = edge.relationship
                        lists[edge.relationship - 1].append(node)
                # elif node.type is None and edge.relationship == 1:
                #     node.parent = self.id
                #     node.hops = self.hops + 1
                #     node.type = edge.relationship
                #     lists[edge.relationship- 1].append(node)

        self.hops = 0

