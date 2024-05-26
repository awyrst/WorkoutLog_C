#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wkt_database.h"

//functions
Database *db_create(){
//allocate heap memory for the database struct and its rcrdarray
	Database *db = malloc(sizeof(Database));
	db->rcrdarray = calloc(4, sizeof(Record));
//initialize other fields
	db->capacity = 4;
	db->size = 0;
	return db;
}

void grow_array(Database * db) {
	//allocate memory for new rcrdarray with double the size
	Record *rcrdptr = calloc(2*(db->size), sizeof(Record)); 
	// copy all the elements
		for(int i = 0;i<(db->size); i++) {
		*(rcrdptr+i) = *((db->rcrdarray)+i);
		}
	// free up the memory for previous record array
	free(db->rcrdarray);
	// then assign array to the new rcrdarray field
	db->rcrdarray = rcrdptr;
	db->capacity = 2*db->capacity;

}

void db_append(Database * db, Record const * item) {
//if size is at capacity, double the size of the array
	if(db->size == db->capacity) grow_array(db);
	*((db->rcrdarray)+(db->size)) = *item;
	db->size++;
}

void db_remove(Database *db) {

	db->size--;
}

Record * db_index(Database * db, int index) {
//check if the index is out of bounds	
	if((db->size) <= index) {
	puts("Error: index out of bounds");
	//return NULL
	return NULL;

	} else {
//otherwise return pointer to record at index
	return (db->rcrdarray)+index;	
	}
}

Record * db_lookup(Database * db, char const * exercise, int sets, int reps, char const * weight) {

	for(int i = 0; i<(db->size); i++) {
	//if the record at index i has the desired handle
		if(strcmp((*((db->rcrdarray)+i)).exercise, exercise) == 0 && (*((db->rcrdarray)+i)).sets == sets && (*((db->rcrdarray)+i)).reps == reps && strcmp((*((db->rcrdarray)+i)).weight, weight) == 0) {
		//return the pointer to that record
		return (db->rcrdarray)+i;	
		}		
	}
puts("There does not exist an entry in the log with the inputted information");
//return NULL
return NULL;
}


void db_free(Database * db) {
free(db->rcrdarray);

free(db);
}

void print_record(Record const *r) {
	char d[16];
	char s[16];
struct tm *boop1 = localtime(&(r->timestamp));
strftime(s, 16, "%F", boop1);
strftime(d, 16, "%R", boop1);
	
printf("%s %d %d %s %s %s %s\n", r->exercise, r->sets, r->reps, r->weight, s, d, r->comment);
}

void print_database(Database * db) {
	for(int i = 0; i<(db->size); i++) {
	print_record((db->rcrdarray)+i);
	}
}

Record parse_record(char *line) {

//use strtok multiple times to parse the line into strings delimited by commas
char *exercisestr = strtok(line, ",");
char *setsstr = strtok(NULL, ",");
char *repsstr = strtok(NULL, ",");
char *weightstr = strtok(NULL, ",");
char *commentstr = strtok(NULL, ",");
char *timestampstr = strtok(NULL, ",");

//once you have the four strings, turn the follower count and timestamp into long unsigned ints
int sets = strtol(setsstr, NULL, 10);
int reps = strtol(repsstr, NULL, 10);
long unsigned int timestamp = strtoul(timestampstr, NULL, 10);

//instantiate a record, and populate it with the new variables

Record r;
 
strcpy(r.exercise, exercisestr);
r.sets = sets;
r.reps = reps;
strcpy(r.weight, weightstr);
r.timestamp = timestamp;
strcpy(r.comment, commentstr);
return r;
//return the record
}	

void db_load_csv(Database * db, char const * path) {
//create the stream using fopen
FILE *data = fopen(path, "r+");

	//make sure file exists, otherwise create it
	if( data == NULL ) {
	data = fopen(path, "w+");

	}

/* getline():
 * we initially pass a null-pointer and size of zero to getline() so that it allocates a new buffer
 * for reading from the "data" stream. getline() updates the line variable to point to the allocated
 * buffer, and updates the size variable to be the size of the buffer. The number of bytes written 
 * into the buffer is stored in the variable nread, which is always less than the buffer's capacity
 * since newline will dynamically update the capacity of the buffer to be the size that we need.
 * The size variable stores buffer's capacity. Must always use free() after we're done with the buffer. */

//on the call to getline(), line points to the newly allocated buffer which contains a line extracted from 
//data. Subsequent calls to getline with the same arguments will keep using the same buffer we allocated
//on the first iteration of the loop, updating the 'size' and 'nread' variables accordingly.

char *line = NULL;
size_t size;
size_t nread;

//nread will only be '-1' if getline failed, which happens on EOF condition or some other issue occured
Record r;
while( (nread = getline(&line, &size, data)) != -1){
//parse the line into a record, and add it to the database in one step

r = parse_record(line);
db_append(db, &r);

} 
//the entire csv file has been parsed into records which were appended to the database. 
//We now free up the buffer.
free(line);
fclose(data);
}

void db_write_csv(Database * db, char const * path) {

FILE *data = fopen(path, "w+");
Record *r;
for(int i = 0; i<(db->size); i++) {
r = db_index(db, i);
fprintf(data,"%s,%d,%d,%s,%s,%lu\n", r->exercise, r->sets, r->reps, r->weight, r->comment, r->timestamp);
}

fclose(data);
}
