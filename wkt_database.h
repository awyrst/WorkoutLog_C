#ifndef DB_H
#define DB_H

typedef struct Record { 

	   
int sets;

int reps;

long unsigned int timestamp;

char exercise[32];

char weight[7];

char comment[64];


} Record;

typedef struct Database { 

int capacity;

int size;

Record *rcrdarray;


} Database;

Database *db_create();

void db_append(Database * db, Record const * item);

void db_remove(Database *db);

Record * db_index(Database * db, int index);

Record * db_lookup(Database * db, char const * exercise, int sets, int reps, char const *weight);


void db_free(Database * db);

void db_load_csv(Database * db, char const * path);

void db_write_csv(Database * db, char const * path);

#endif
