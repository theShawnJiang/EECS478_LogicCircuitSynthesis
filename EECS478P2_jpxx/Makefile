####  Makefile 	####
# Usage:           #
#   >make          #
#   >make clean	   #
####################

FILES =  truthTable.cpp circuit.cpp library.cpp modules.cpp datapaths.cpp main.cpp

all: 
	g++ -O3 -Wall -lm -g -o project2 ${FILES}

backup:
	cp -rf *.c *.h Makefile backup/*

clean:
	rm -f project2 *~
