#include "inputHandlers.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Input: receives 2 strings, one representing a line of text, and the other is a destination string
 * Output: returns the amount of chars read.
 *         Extracts the next token (from the first no-space char until a comma or space is reached) and stores it in the destination string
 *
 * Algorithm: iterate through line until its end (or until we manually break out of the loop), in each iteration increment the amount of chars read.
 * Skip all the spaces until the command is reached. Then, until either a space or a comma was encountered (in this case we manually break),
 * copy the chars we read to the destination string.
 * */
int get_token(char *line, char *dest){
    int read_chars = 0; /* counter for amount of characters read */
    int reached_token = (isspace(line[0])) ? 0 : 1; /* states whether we have reached the token yet */
    int i; /* used in the for loop */

    for (i = 0; line[i] != '\0'; ++i) { /* scan the line until its end */
        read_chars++; /* update amount of characters read */
        if (reached_token == 0){ /* if we haven't reached the command yet */
            if (isspace(line[i])){ /* continue if the current char is a white space */
                continue;
            } else{ /* if we reached the first char of the token */
                *dest++ = line[i]; /* copy the first char to the destination string */
                reached_token = 1; /* mark that the token was reached */
            }
        } else{ /* this means we have already reached the token */
            if (isspace(line[i]) || line[i] == ','){ /* exit the loop if a space or comma was encountered */
                break;
            }
            *dest++ = line[i]; /* if the token isn't finished yet, copy the current char to the destination string */
        }
    }

    *dest++ = '\0'; /* mark the end of the string */
    return read_chars; /* return the amount of chars read */
}

/* Input: receives a string that represents name of a matrix and an int representing its supposed index in matrices array.
 * Output: returns 0 if the matrix is not legal for our purposes, non-zero value otherwise.
 *
 * Algorithm: check if:
 *              1. the index is in the array bounds (positive and smaller than the length of the array)
 *              2. the name is appropriate (using strncmp) - of format MAT_X
 *              3. there are no extra chars (using strlen)
 *     If one of these conditions is false, 0 ("false") will be returned. Otherwise, a non-zero value will be returned ("true")
 * */
int legal_matrix(char *matrix, int index){
    return !(index < 0 || index >= NUM_OF_MATRICES || strncmp(matrix, "MAT_", MATRIX_NAME_LENGTH-1)) && (strlen(matrix) == MATRIX_NAME_LENGTH);
}

/* Input: receives a string that represents a command.
 * Output: returns 0 if the command isn't legal, non-zero value otherwise.
 *
 * Algorithm: search the command in the string of all the commands using strstr - it will return null if the command isn't there.
 *            Check if it is equal to an empty string or a space.
 *            If one of these is true, return 0 ("false"), else return a non-zero value ("true") */
int legal_command(char *command){
    /* return a non-zero value only if command is one of the available one, not an empty string and not a space */
    return strstr(COMMANDS, command) != NULL && strcmp(command, "") && strcmp(command, " ");
}


/* Input: receives a string that represents a scalar.
 * Output: returns 0 if the scalar isn't a valid one, returns 1 otherwise.
 *
 * Algorithm: skip all white spaces. Decrement ind by one because the spaces check may have skipped the first important char.
 * Check if the scalar is introduced by a sign, and if so continue to the next char. Then until the end of the string is reached,
 * we ensure it is a valid scalar: only one dot, and apart of that only digits.
 * If we encountered a second dot or something that isn't a digit, return 0. If the loop has ended, we return 1 because it means the scalar is valid
 */
int legal_scalar(char *scalar){
    int already_reached_point = 0; /* a double might contain a point in it, but only one point */
    int ind = 0; /* current index to read from scalar */

    for (; isspace(scalar[ind]); ind++); /* skip spaces */
    ind = (ind > 0) ? ind - 1 : ind; /* the for loop has skipped the first "important" char only if there were spaces, so we decrease ind to re-read it if necessary */

    if (scalar[ind] == '-' || scalar[ind] == '+'){
        ind++; /* skip the sign if it exists */
    }

    while (scalar[ind] != '\0'){ /* check every character of the string */
        /* if the current character is a dot after a dot has already been found, or it isn't a dot nor a number, the scalar is invalid so we return 0 */
        if (((scalar[ind] == '.') && already_reached_point != 0) || (!isdigit(scalar[ind]) && (scalar[ind] != '.'))){
            return 0;
        }

        if (scalar[ind] == '.'){ /* check if we reached a dot */
            already_reached_point = 1; /* indicate that another dot isn't legal */
        }

        ind++; /* continue to check the next character */
    }

    return 1; /* if we arrived here, it means the scalar is legal so we return 1 */
}


/* Input: a string as a character pointer
 * Output: returns 0 if there is text other than white chars, 1 if only white chars
 *
 * Algorithm: iterate through all the chars of the string received. If a non-white-space char encountered, return 0.
 * If no such character was encountered throughout the loop, return 1 - "true".
 *
 * */
int no_extra_text(char *str){
    char *c = str; /* a char to iterate over the string */

    while (*c != '\0'){ /* check through all the string */
        if (!isspace(*c++)){ /* if we encountered a text that is not a white space, return 0, "false". Increment c to continue checking the rest of the string */
            return 0;
        }
    }

    return 1; /* if we arrived here, it means all the chars were white spaces, so there is no extraneous text */
}

/* Input: a string representing a filename
 * Output: returns nothing. Prints the file to the screen.
 *
 * Algorithm:
 */
void print_file(char *filename){
    FILE *fp = fopen(filename, "r"); /* open the file in read mode */
    int c; /* current char of the file */

    if (fp == NULL){ /* ensure file opened properly */
        exit(FILE_ERROR);
    }

    while ((c = getc(fp)) != EOF){ /* print all the file to the screen */
        putchar(c);
    }

}
