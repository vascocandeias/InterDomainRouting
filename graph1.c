


    
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