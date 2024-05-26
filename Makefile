workout_notes: wkt_igdb.o wkt_database.o
	gcc -o workout_notes wkt_igdb.o wkt_database.o 

wkt_igdb.o: wkt_igdb.c wkt_database.h
	gcc -c wkt_igdb.c

wkt_database.o: wkt_database.c wkt_database.h
	gcc -c wkt_database.c
