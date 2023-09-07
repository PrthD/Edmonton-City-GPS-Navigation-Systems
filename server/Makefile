# ---------------------------------------------------
# Name: Parth Dadhania
# SID: 1722612
# CCID: pdadhani
# AnonID: 1000330704
# CMPUT 275, Winter 2023
# Assessment: Assignment - Navigation System (Part 2)
# ---------------------------------------------------

server: dijkstra.o digraph.o server.o
	g++ -o server server.o digraph.o dijkstra.o

dijkstra.o: dijkstra.cpp dijkstra.h
	g++ -c dijkstra.cpp

server.o: server.cpp dijkstra.o digraph.o
	g++ -c server.cpp

digraph.o: digraph.cpp digraph.h wdigraph.h
	g++ -c digraph.cpp

clean:
	rm *.o server