#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "wkt_database.h"
#define min(a,b) (((a) < (b)) ? (a) : (b))

//function removes newline character from end of a string
void rmbackslashn(char *line){

*(line+strlen(line)-1) = '\0';

}

//function returns 0 if there were no additional arguments, 1 otherwise
int otherargs() {

char *extrainput;

	if((extrainput = strtok(NULL, " "))==NULL) {
	return 0;
	} else return 1;
	
}

int exponent(int base, int power) {

int n=1;

for(int i = 1; i<=power; i++){

	n=n*power;
}

return n;
}

int strtoint(char* string){
int i;
int n=0;
	//the number is positive
	for(i=0;i<strlen(string);i++){
	if(*(string+i)-'0' < 0 || *(string+i)-'0' > 9){
	
	return -1;
	}
	n = n+((*(string+i)-'0')*exponent(10, (strlen(string)-i-1)));

	}
return n;
}

//two functions below work together to print the list of records from the database
void printf_record(Record const *r) {
	char d[16];
	char s[16];
struct tm *boop1 = localtime(&(r->timestamp));
strftime(s, 16, "%F", boop1);
strftime(d, 16, "%R", boop1);

printf("\b| %-20.20s | %-4d | %-4d | %-7.7s | %s %s | %.64s\n", r->exercise, r->sets, r->reps, r->weight, s, d, r->comment);
}

void printf_database(Database * db) {

printf("    | EXERCISE             | SETS | REPS | WEIGHT  | LAST MODIFIED    | COMMENT\n");
printf("----|----------------------|------|------|---------|------------------|----------------------------------------------------------------\n");

	for(int i = 1; i<=min((db->size),9999); i++) {
		printf("%-4d ", i);
	printf_record((db->rcrdarray)+i-1);
		
		if(i == 9999) {
		printf("\nmaximum number of entries printed\n");
		}
	}
}

//functions used for 'add' and 'update' to validate input
//first function checks if there is a comma in the string - returns 1 if there is comma
int iscomma(char const *string){

	for(int i=0;i<strlen(string);i++){
	
		if(*(string+i) == ',') return 1;
	}

	return 0;
}

//second function validates the exercise name - returns 1 if valid, 0 otherwise
int validate_exercise_weight(char const *exercise, char const *weight){

	
	if(iscomma(exercise)) {
	printf("Error: exercise name must not contain a comma\n");
	return 0;
	}	

	if(strlen(exercise) > 32) {
	printf("Error: exercise name too long\n");
	return 0;
	}

	if(iscomma(weight)) {
	printf("Error: weight parameter must not contain a comma\n");
	return 0;
	}	

	if(strlen(weight) > 7) {
	printf("Error: Either you inputted an amount of weight that doesn't make sense or you're crazy strong\n");
	return 0;
	}

return 1;
}
//third funtion validates the set/rep count - returns 1 if valid, 0 otherwise
int validate_sets_reps(char const *sets, char const *reps){
	//first make sure the string only contains numbers
	for(int i=0;i<strlen(sets);i++){
		if(*(sets+i)-'0' < 0 || *(sets+i)-'0' > 9){
		printf("Error: set count must not contain special characters\n");
		return 0;
		}
	}
	//now make sure the number is in range
	if(strtoul(sets, NULL, 10) > 9999){
	printf("Error: set count is too large\n");
	return 0;
	}

	for(int i=0;i<strlen(reps);i++){
		if(*(reps+i)-'0' < 0 || *(reps+i)-'0' > 9){
		printf("Error: rep count must not contain special characters\n");
		return 0;
		}
	}
	//now make sure the number is in range
	if(strtoul(reps, NULL, 10) > 9999){
	printf("Error: rep count is too large\n");
	return 0;
	}

return 1;
}

//fourth funtion validates the comment - returns 1 if valid, 0 otherwise
int validate_comment(char const *comment){

	if(iscomma(comment)) {
	printf("Error: comment must not contain a comma\n");
	return 0;
	}	

	if(strlen(comment) > 64) {
	printf("Error: comment too long\n");
	return 0;
	}
return 1;

}
//fifth function to prompt user for the comment provided that they used the add function with
//valid handle and follower count. Returns 0 on success, 1 otherwise.
int prompt_comment(char *line, size_t size, size_t nread){

printf("Comment> ");

	if((nread = getline(&line,&size,stdin)) == -1) return 1;
//remove the newline character of the input
rmbackslashn(line);

//since the comment is allowed to have spaces, and the newline character as already been removed
//we can just let the comment variable point to the string in the buffer

//now validate the comment using helper function. Helper function prints the appropriate
//error messages
	if(validate_comment(line) == 0) return 1;
	
return 0;
}

//sixth function is called when uses first input is update/add. It uses strtok to extract the next three 
//tokens (exercise, sets and reps), and validates them using the above functions. This function also checks
//to make sure there exactly three additional arguments. Returns 1 on success, zero otherwise
int validate_input(char *exercise, char *sets, char *reps, char *weight){
	
	//validate handle and followers input
	if(validate_exercise_weight(exercise, weight) && validate_sets_reps(sets, reps)){
	//input checks out, return success code	
	return 1;	
	}
//input is not valid, return failure

return 0;
}


//return 1 if success, 0 otherwise
int add(Database * db, char const *exercise, char const *sets, char const *reps, char const *weight, char const *comment){

Record *r = malloc(sizeof(Record));

strcpy(r->exercise, exercise);
r->sets = strtol(sets, NULL, 10);
r->reps = strtol(reps, NULL, 10);
strcpy(r->weight, weight);
r->timestamp = time(NULL);
strcpy(r->comment, comment);

db_append(db, r);
free(r);
return 1;
}

//return 1 on success, 0 on failure
int update(Database * db, int index, char const *exercise, char const *sets, char const *reps, char const *weight, char const *comment){

	Record *r = db_index(db, index-1);
//db_index returns null if there was an index out of bounds
	if(r != NULL) {

	strcpy(r->exercise, exercise);
	r->sets = strtol(sets, NULL, 10);
	r->reps = strtol(reps, NULL, 10);
	strcpy(r->weight, weight);
	
	r->timestamp = time(NULL);
	strcpy(r->comment, comment);
	return 1;
	}
return 0;
}


int main_loop(Database * db, char *path) {


int flag = 0;
char *line = NULL;
size_t size;
size_t nread;
	while(1) {
	printf("> ");
		
		if((nread = getline(&line,&size,stdin)) == -1) break;
	//remove the newline character of the input
	rmbackslashn(line);

	//grab the first input word using strtok, and validate it
	char *input1 = strtok(line, " ");

	//depending on the first input word, check what the second one is
		//first input is list
		if(strcmp("list",input1)==0) {

			if(otherargs()) {
			printf("Error: there must be no additional arguments to 'list' function\n");

			} else {
			//do the stuff that list is supposed to do
			printf_database(db);

			}
			
		
		//first input is add
		} else if(strcmp("add",input1)==0) {
		char *exerciseref = strtok(NULL, " ");
		char *setsref = strtok(NULL, " ");
		char *repsref = strtok(NULL, " ");
		char *weightref = strtok(NULL, " ");

			if((exerciseref == NULL) || (setsref == NULL) || (repsref == NULL) || (weightref == NULL) ||  otherargs() == 1) {
			printf("Error: usage: <add> <EXERCISE> <SETS> <REPS> <WEIGHT>\n");
			} else {	
			char *exercise = strdup(exerciseref);
			char *sets = strdup(setsref);	
			char *reps = strdup(repsref);	
			char *weight = strdup(weightref);
				if(validate_input(exercise, sets, reps, weight) == 1) {
				//appropriate error messages were printed in helper function in case of failure
		//do what 'add' function is supposed to do
		//call function to prompt user for comment, and validate that input as well
		
		//the function called below will print the appropriate error messages, so
		//we only need its return value here.
						
					if(prompt_comment(line, size, nread) == 0) {
					//appropriate error messages were printed in helper function in case
					//of failure.
					//prompt_comment was successful, add the record			
					char *comment = line;
					flag = add(db, exercise, sets, reps, weight, comment);	
								
					}
				}
				//don't have to worry about freeing comment since it will 
				//get reallocated on next call to getline()
			free(exercise);
			free(sets);
			free(reps);
			free(weight);

			}
		//first input is update
		} else if(strcmp("update",input1)==0) {

		char *indexref = strtok(NULL, " ");
		char *exerciseref = strtok(NULL, " ");
		char *setsref = strtok(NULL, " ");
		char *repsref = strtok(NULL, " ");
		char *weightref = strtok(NULL, " ");

			if((indexref == NULL) || (exerciseref == NULL) || (setsref == NULL) || (repsref == NULL) || (weightref == NULL) || otherargs() == 1) {
			printf("Error: usage: <update> <INDEX> <EXERCISE> <SETS> <REPS> <WEIGHT>\n");

			} else {	

			int index = strtoint(indexref);

				if(index == -1 || db->size < index) {
				puts("Error: index is an invalid number or is out of bounds");
		
				} else {
			
			//we must create copies of the handle and follower variables or else they get overwritten on
			//subsequent call to getline when prompting for the comment	
				char *exercise = strdup(exerciseref);
				char *sets = strdup(setsref);	
				char *reps = strdup(repsref);	
				char *weight = strdup(weightref);
					if(validate_input(exercise, sets, reps, weight) == 1) {
					//appropriate error messages were printed in helper function in case of failure
			//do what 'update' function is supposed to do
			//call function to prompt user for comment, and validate that input as well
			
			//the function called below will print the appropriate error messages, so
			//we only need its return value here.
							
						if(prompt_comment(line, size, nread) == 0) {
						//appropriate error messages were printed in helper function in case
						//of failure.
						//prompt_comment was successful, add the record			
						char *comment = line;
						flag = update(db, index, exercise, sets, reps, weight, comment);	
					
						}
					}
					//don't have to worry about freeing comment since it will 
					//get reallocated on next call to getline()
					
				free(exercise);
				free(sets);
				free(reps);
				free(weight);

					
				}
			}
		} else if(strcmp("save",input1)==0) {

			if(otherargs(line)) {
			printf("Error: there must be no additional arguments to 'save' function\n");

			} else {
			//do the stuff that save is supposed to do
			db_write_csv(db, path);

			printf("wrote %d entries\n", db->size);
			//do something with boolean flag here*
			flag = 0;
			}

		//first input is exit
		} else if(strcmp("exit",input1)==0) {

			char *input2 = strtok(NULL, " ");
			
			//if second input is not null, perform checks	
			if(input2 != NULL){
				if(strcmp(input2, "fr")==0 && otherargs(line)==0){
					break;
				} else {
				printf("Error: Usages: <exit> OR <exit fr>\n");
				}

			} else {
			//do the stuff that exit is supposed to do
						
			//if boolean flag is set to on, print an error
				if(flag) {
				printf("Error: you did not save your changes. Use 'exit fr' to force exiting anyway\n");
				} else break;
			}
		} else if(strcmp("remove",input1)==0) {

			if(otherargs(line)) {
			printf("Error: there must be no additional arguments to 'remove' function\n");

			} else {
			//do the stuff that remove is supposed to do

			db_remove(db);
			printf("removed most recent entry\n");

			//do something with boolean flag here*
			flag = 1;
			}
			

		} else	{
		printf("Error: '%s' is not a supported operation\n", input1);
		
		}
	}
	//what if free freed line but it never got allocated a buffer in the first place (i.e. getline failed)?
	
if(line != NULL) free(line);

}



//function for validating the input name of the csv file
int validate_wkt_date(char *filedate){

	//first validate length of date input
	if(strlen(filedate) != 10) {
	printf("Error: date must be in 'yyyy-mm-dd' format\n");
	return 0;
	}
	
	//now validate the date	
	if(*(filedate+4) != '-' || *(filedate+7) != '-') {
	printf("Error: incorrectly formatted date\n");
	return 0;
	}
	//check the year, month and day separately
	char year[5];
	char month[3];
	char day[3];

	strncpy(year, filedate, 4);
	strncpy(month, filedate + 5, 2);
	strcpy(day, filedate + 8);


	int yyyy = strtoint(year);
	int mm = strtoint(month);
	int dd = strtoint(day);

	if( (0 > yyyy) || (yyyy > 9999) || (1 > mm) || (mm > 99) || (1 > dd) || (dd > 99) ) {
	printf("Error: incorrectly formatted date\n");

	return 0;
	}
	return 1;

}


int main() {
		
//first validate that there were two additional arguments, and that the date was formatted correctly
//format is "./workoutnotes <workout_name> <yyyy-mm-dd>" --> "workout_name_yyyy-mm-dd_.csv"
//

system("[ ! -d ./workout_log_dir ] && mkdir ./workout_log_dir && echo created directory 'workout_log_dir' to store workout logs");
	
puts("\nWorkout log program. This program allows you to create, view and edit notes about your workouts.");
char *line = NULL;
size_t size;
size_t nread;
	while(1) {

puts("\nEnter the name of the workout you would like to create/edit.");
puts("Note that the name of the workout will be truncated at 20 characters.");
puts("Enter 'quit' if you would like to exit the program.\n");

	//if buffer for line cannot be allocated, exit the loop	
		if((nread = getline(&line,&size,stdin)) == -1) break;
		//remove the newline character of the input
		rmbackslashn(line);

		char wktname[21];
		strncpy(wktname,line,20);
	//if user inputted 'quit', exit the program		
		if(strcmp(wktname,"quit")==0) break;

	//now we can read the date
		puts("\nEnter the date of the workout log you would like to create/edit\n");

		if((nread = getline(&line,&size,stdin)) == -1) break;
		//remove the newline character of the input
		rmbackslashn(line);
	
		
	//now validate that the date was inputted correctly	
		if (validate_wkt_date(line)) {
		
			//inputs are valid, now make the directory if it hasn't already been made
			

			char filename[55];

			strcpy(filename, "./workout_log_dir/");
			strcat(filename, wktname);
			strcat(filename, "_");
			strcat(filename, line);
			strcat(filename, "_.csv");
			
			Database *db = db_create();

			db_load_csv(db, filename);

			printf("\nLoaded %d Entries.\n\n", db->size);

			printf_database(db);

			main_loop(db, filename);

			db_free(db);
		}
	}
//what if free freed line but it never got allocated a buffer in the first place (i.e. getline failed)?
if(line != NULL) free(line);

}
