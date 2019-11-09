# ADRC - Project 2
# Main function
#
# by: Sofia Estrela (84186)
#     Vasco Candeias (84196)
#
# 29 November 2019
#
###############################

# import from the other files of this project
from IO import read
from Graph import *
import sys

# import the prefix table into a tree
try:
    graph = Graph()
    graph.initGraph(sys.argv[1])

except:
	print("There was an error opening the file. Starting with a blank table...")
	exit()

# Menu printing and get the user input
while(1):
	print()
	print("Options:")
	print("\t0) Comercial paths statistics")
	print("\t1) Shortest paths statistics")
	print("\t2) Quit")
	val = read("\n> ")
	print()

    