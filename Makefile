# ---------------------------------------------------
# Name: Parth Dadhania
# SID: 1722612
# CCID: pdadhani
# AnonID: 1000330704
# CMPUT 275, Winter 2023
# Assessment: Assignment - Navigation System (Part 2)
# ---------------------------------------------------

run:	
	gnome-terminal -- ./server/server
	python3 client/client.py

all:
	#make compile
	make run
	
clean:
	(cd server; make clean)
	rm -f inpipe
	rm -f outpipe

compile:
	(cd server; make)