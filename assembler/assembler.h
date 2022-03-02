#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "linkedlist.h"

typedef struct opcode_word {
    unsigned int opcode : 16;
    unsigned int E : 1;
    unsigned int R : 1;
    unsigned int A : 1;
    unsigned int msb : 1;
} opcode_word;

typedef struct other_word {
    unsigned int destination_adress : 2;
    unsigned int destination_register : 4;
    unsigned int source_adress : 2;
    unsigned int source_register : 4;
    unsigned int function : 4;
    unsigned int E : 1;
    unsigned int R : 1;
    unsigned int A : 1;
    unsigned int msb : 1;
} other_word;

enum registers {
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15
};

enum command_names {
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    JSR,
    RED,
    PRN,
    RTS,
    STOP
};

/*
 * This function looks in the folder for the file with the filename with the file extension .as
 * *filename : the filename we're searching for
 */
void look_for_file(char * filename);

/*
 * This function looks for the macro keyword in the file, and in the case if does find it, looks also for the keyword endm.
 */
void check_macro(void);

/* Duplicates file stored in fd1 into fd2
 * *filename : the name of the file that will be duplicated
 */
void duplicate_file(char * filename);

/*
 * This function takes a string and returns a char pointer to the first string character after all white space at the start of the string
 * Returns a char pointer to the first string character after all white space
 * input[] : the string where it shall skip the white space at the start
 */
char *skip_white_space_at_start(char input[]);

/*
 * This function takes a string and removes all white spaces from the entire string, leaving only regular characters
 * input[] : the string where all white space will be deleted
 */
void delete_spaces(char input[]);

/*
 * This function is used after a macro keyword has been found. It goes over the .am file and replaces every appearance of the found keyword to the proper macro.
 * *list : a list containing the macro which will be put in the .am file
 * line_number : the number of the line which contains the macro keyword
 */
void replace_macro(linked_list * list, int line_number);

/*
 * This function looks for instances of the  macro keyword in the file text.
 * *list : a list containing the macro which will be put in the .am file
 */
void find_macro_instances(linked_list * list);