# WorkoutLog_C
Workout Log Program in C

General Information about what the program does and how it appears to the user:

This program's executable is called workout_notes.

This is an interactive program that manages information about your workouts and encodes the info in .csv files.

The program starts by creating a subdirectory called 'workout_log_dir' (if it doesn't exist already) to store the
.csv files containing the information about the user's workouts. Each .csv file is named with the following convention:

(workout_name)_(workout date in yyyy-mm-dd format)_.csv

For example, 'legday_2024-02-23_.csv' is the .csv file containing the workout information about the workout named
'legday'. This workout took place on February 23rd, 2024.

The program then prompts the user for the name and date of the workout to be edited. If the information
inputted by the user doesn't correspond to a workout that has been edited already, the program creates the
corresponding .csv file in the workout_log_dir subdirectory with the convention outlined above.

The user is then visually presented with all the entries in the workout, and at this stage, the user is free
to make changes using the 'add', 'update', and 'remove' commands. The 'list' command will visually display all
the entries in the workout, the 'save' command will save the changes to the appropriate .csv file, and the 'exit'
command will return the user to the previous step, where they are prompted once again to edit a workout.

An entry in the workout log contains several fields. These are: the exercise performed, the number of sets done with
the specified exercise, the number of repetitions per set, the weight lifted on each repetition, any comments about
the exercise, and a timestamp which indicates when the entry was made to the workout log.

If you want more details about how to use the 'update', 'add', and 'remove' commands, I suggest running the program
for yourself and trying them out. There are fairly detailed error messages that will guide you on how to use these 
commands properly.

Some notes about the Implementation:

When the user is prompted to edit a workout, the information for a given workout is loaded into a 'database' data structure.
This data structure is supported by two main structs in C. The first struct is called 'Record' which represents an entry in
the workout log (and also technically one record in the .csv file), and the second is called 'Database' which contains three
fields: an int called capacity, an int called size, and an array of the 'Record' structs.

The wkt_database.h file contains the definitions for these structs and the signatures for their helper functions. The
wkt_database.c file contains the descriptions for these helper functions, and the wkt_igdb.c file contains the main function.
The contents of the Makefile and its purpose should be self-explanatory.

Feel free to study these files if you're interested - I've placed comments throughout the code to help explain what each 
function does and how they work.

Final Notes:

This program is inspired by and an expansion upon the 4th assignment for a course I took at McGill University in the Winter 2024 Semester
called: COMP 206 - introduction to software systems. I really enjoyed this course and it has helped me start taking my
programming more seriously (to the point where I've created projects like this on my own, and made this GitHub account and repo 
in the first place).
