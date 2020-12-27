#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "inputHandlers.h"

#define HELP_FILE "help.txt"

/* a constant to use whenever the program exits and needs to free all the dynamically allocated resources */
#define FREE_ALL free(line_holder); \
                free(command); \
                free(matrix); \
                free(second_matrix); \
                free(result_matrix); \
                free(scalar);

/* check if extra text was entered
 * if so, inform the user and skip further processing */
#define CHECK_EXTRA_TEXT if (!no_extra_text(line)){ \
                            printf("Extraneous text after end of command!\n"); \
                            continue; \
                        }

/* free the dynamically allocated variables and exit with an error code */
#define CHECK_MEMORY_ALLOC(x) if (x == NULL) { \
                                    printf("Memory allocation error. Terminating!\n"); \
                                    FREE_ALL \
                                    exit(MEMORY_ALLOC_ERROR); \
                                }

/* mat.h is included from inputHandlers.h */

int main(int argc, char *argv[]) {
    char *line_holder = (char *) malloc(MAX_BUF * sizeof(char)); /* variable to store the user's entire command */
    char *line; /* used to iterate through line, so that we would be able to free() the resources later  */
    char *command = (char *) malloc(MAX_COMMAND_LENGTH * sizeof(char)); /* variable to store the command name */
    int i; /* used in a for loop to initialize all matrices and reset them to all-0 */
    int read; /* stores the value returned from get_token, indicates how many characters were read */
    int index, second_index, third_index; /* all array indexes for the first, second and third matrix respectively */
    char *matrix, *second_matrix, *result_matrix; /* names for the first, second and third matrix respectively */
    char *scalar;  /* to hold the scalars in the read_mat command and the mul_scalar command */
    int read_mat_no_comma = 0; /* if there is no comma after read_mat, we should wait and see whether it is a comma problem or no args problem */
    int three_mats_comma_problem = 0; /* if there is not a comma after a matrix in a command where there are 3 matrices, we should wait because it might be argument problem as well. */

    mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F; /* create all the needed matrices */

    /* create an array of the matrices for quick and easy access. Fill it with the created matrices */
    mat *all[NUM_OF_MATRICES];
    all[0] = &MAT_A;
    all[1] = &MAT_B;
    all[2] = &MAT_C;
    all[3] = &MAT_D;
    all[4] = &MAT_E;
    all[5] = &MAT_F;

    if (argc > 1){
        print_file(HELP_FILE);
        exit(0);
    }

    /* allocate memory for the different variables to be used */
    matrix = (char *) malloc( MATRIX_NAME_LENGTH * sizeof(char)); /* allocate memory for the matrix name */
    second_matrix = (char *) malloc( MATRIX_NAME_LENGTH * sizeof(char)); /* allocate memory for the second matrix */
    result_matrix = (char *) malloc(MATRIX_NAME_LENGTH * sizeof(char)); /* allocate memory for the result matrix */
    scalar = (char *) malloc(MAX_NUM_LENGTH * sizeof(char)); /* allocate memory for the current scalar scalar */

    /* ensure all memory allocation are done correctly. If not, free all the dynamically allocated variables and exit with an error code */
    CHECK_MEMORY_ALLOC(matrix)
    CHECK_MEMORY_ALLOC(second_matrix)
    CHECK_MEMORY_ALLOC(result_matrix)
    CHECK_MEMORY_ALLOC(scalar)
    CHECK_MEMORY_ALLOC(line_holder)
    CHECK_MEMORY_ALLOC(command)

    for (i = 0; i < NUM_OF_MATRICES; ++i) { /* iterate through all the matrices and reset them */
        reset_mat(*all[i]);
    }

    while (1){ /* continue asking the user for input until he enters "stop" or EOF encountered */
        fputs("\nPrompt> ", stdout); /* inform the user the program is ready to receive input */

        /* get the input from the user and store it in the line variable */
        if (fgets(line_holder, MAX_BUF, stdin) == NULL){
            /* fgets will return NULL if we encountered an EOF signal. Inform the user and exit. */
            printf("EOF reached. Terminating...\n");
            exit(EOF_ENCOUNTERED_ERROR); /* exit with an error code */
        }

        read_mat_no_comma = 0; /* reset the variable */
        three_mats_comma_problem = 0; /* reset the variable */

        line = line_holder; /* store the "head" of the pointer to free it properly */

        read = get_token(line, command); /* extract the command from the input line, store how many characters were read */
        line += read - 1; /* skip the already read parts, but check the delimiter (space or comma) */

        if (!legal_command(command)){ /* ensure the entered command is legal */
            if (!strcmp(command, "")){ /* it is okay if no command was entered, just skip further processing */
                continue;
            }
            /* if the command is not legal, inform the user and continue to the next iteration of the while loop */
            /* printf("Sorry, undefined command name!\n"); */
            print_file(HELP_FILE);
            continue;
        }

        /* the get_token function will stop if it encountered a white space or a comma only
         * thus, every time we want to check the delimiter between arguments, if it isn't a whitespace we know it is a comma
         */
        if (!isspace(*line++)){ /* ensure the delimiter after the command is a space, not a comma, and skip it anyway */
            printf("Illegal comma!\n"); /* inform the user */
            continue; /* skip the next processing and continue to the next iteration */
        }

        /* check which command the user entered */
        if (strcmp(command, "stop") == 0){ /* exit the loop if the user entered "stop" */
            /* check if there was extra text entered. If so, inform the user and continue to the next iteration of the while loop */
            CHECK_EXTRA_TEXT
            break;
        }

        if (strcmp(command, "help") == 0){ /* show informative file if requested */
            print_file(HELP_FILE);
            continue;
        }

        read = get_token(line, matrix); /* read the matrix name and store it in matrix */
        line += read - 1; /* skip the characters we have just read, but check the delimiter (space or comma) */

        if (isspace(*line++) && strcmp(command, "print_mat")){ /* ensure the delimiter after the matrix is a comma, not a space, and skip it anyway */
            if (!strncmp(command, "mul", strlen("mul")) || !strncmp(command, "add", strlen("add")) || !strncmp(command, "sub", strlen("sub"))){
                three_mats_comma_problem = 1;
            }
            else if (strcmp(command, "read_mat")){ /* display the message only if the command isn't read_mat */
                printf("Missing comma!\n"); /* inform the user */
                continue; /* skip the next processing and continue to the next iteration */
            } else {
                /* if the command is read_mat, we should wait and see whether arguments were provided at all to determine the right message */
                read_mat_no_comma = 1;
            }
        }

        index = matrix[MATRIX_NAME_LENGTH-1] - 'A'; /* extract the matrix index using the last letter */
        if (!legal_matrix(matrix, index)){ /* verify the matrix is legal: legal and existing name */
            /* if the matrix isn't legal, inform the user and continue to the next iteration of the while loop */
            printf("Sorry, undefined matrix name!\n");
            continue; /* skip the further processing */
        }

        if (strcmp(command, "read_mat") == 0){ /* if the command is read_mat we should put the values in the matrix */
            double *vals = (double *)malloc(sizeof(double)); /* dynamic array to hold the read_mat values */
            int length = 0; /* holds the length of the array */
            int comma_problem = 0; /* the inner while loop will update it if an missing comma was encountered */
            int error = 0; /* the inner while loop will change it if an error was encountered */

            read = get_token(line, scalar); /* read the first scalar */
            line += read - 1; /* skip the read characters except the delimiter which we want to check */

            while (strcmp(scalar, "")){ /* read all the scalars */
                if (!legal_scalar(scalar)){/* verify the scalar is legal: a real number */
                    error = 1; /* mark that there was an error */
                    break; /* skip the further processing */
                }

                if (isspace(*line++)){ /* if the delimiter is a space, increment comma_problem */
                    comma_problem++; /* count comma problems */
                }

                vals[length++] = atof(scalar); /* add the current scalar to the array */
                vals = (double *)realloc(vals, sizeof(double) * (length+1)); /* reallocate memory */
                read = get_token(line, scalar); /* read the next scalar */
                line += read - 1; /* skip the read characters except the delimiter which we want to check */
            }

            if (length == 0 && !error){ /* check if no scalars at all were entered (including invalid ones) */
                /* if no arguments were inserted, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, missing argument!\n");
                continue;
            }

            if (error){
                /* if one of the scalars wasn't legal, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, one (or more) of your arguments is not a real number!\n");
                continue;
            }

            line--; /* to check the last char read */
            if (!isspace(*line) && !isdigit(*line++)){ /* check the delimiter. If it is not a space nor a digit, it is a comma and thus - error */
                printf("Extraneous text after end of command!\n");
                continue;
            }

            /* ensure the delimiter after each argument is a comma, not a space, except for the last one.
             * comma problem should be 1, since the last argument should have no following comma.
             * Also, check if there was no comma after the matrix name as we checked earlier */
            if (comma_problem > 1 || read_mat_no_comma){
                printf("Missing comma!\n"); /* inform the user */
                continue; /* skip the next processing and continue to the next iteration */
            }

            read_mat(*all[index], vals, length); /* enter the values to the matrix */
            continue; /* we have executed the current command, we should continue to the next while loop iteration */
        } else if (strcmp(command, "print_mat") == 0) { /* if the command is print_mat we should print the matrix we extracted */
            /* ensure no extra text was entered. If so, skip to the next iteration of the while loop */
            CHECK_EXTRA_TEXT
            print_mat(*all[index]);
            continue; /* we have executed the current command, we should continue to the next while loop iteration */
        }

        if (strcmp(command, "mul_scalar") == 0) { /* if the command is mul_scalar we should get the scalar first and then the result matrix */
            read = get_token(line, scalar); /* read the scalar and store it in its variable */

            if (!legal_scalar(scalar)){/* verify the scalar is legal: a real number */
                /* if the scalar isn't legal, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, argument is not a real number!\n");
                continue; /* skip the further processing */
            }

            line += read - 1; /* skip the characters we have just read */
            if (*line++ != ','){ /* ensure that there was a comma between the scalar and the result_matrix */
                printf("Missing comma!\n"); /* inform the user */
                continue; /* skip the next processing and continue to the next iteration */
            }

            read = get_token(line, result_matrix); /* read the result matrix and store it in its variable */
            second_index = result_matrix[MATRIX_NAME_LENGTH - 1] - 'A'; /* extract the result matrix */
            line += read - 1;

            if (!legal_matrix(result_matrix, second_index)) { /* verify the matrix is legal: legal and existing name */
                /* if the matrix isn't legal, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, undefined matrix name!\n");
                continue; /* skip the further processing */
            }

            if (!isalpha(*line)){
                /* ensure no extra text was entered. If so, skip to the next iteration of the while loop */
                CHECK_EXTRA_TEXT
            }

            /* if both the scalar and the two matrices are legal, execute the mul_scalar method, convert the scalar to a double using atof */
            mul_scalar(*all[index], atof(scalar), *all[second_index]);

            continue; /* we have executed the current command, we should continue to the next while loop iteration */
        }

        /* if we have skipped those three commands, it means we have at least 2 matrices one after the other, so we extract another matrix */

        read = get_token(line, second_matrix); /* read the second matrix and store it in its variable */
        line += read - 1; /* skip the characters we have just read, but check the delimiter */
        second_index = second_matrix[MATRIX_NAME_LENGTH-1] - 'A'; /* extract the second matrix's index */

        if (!legal_matrix(second_matrix, second_index)) { /* verify the matrix is legal: legal and existing name */
            if (!strcmp(second_matrix, ",")){
                /* if the matrix is a comma, it means there were multiple consecutive commas, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, Multiple consecutive commas!\n");
            } else {
                /* if the matrix isn't legal, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, undefined matrix name!\n");
            }

            continue; /* skip the further processing */
        }

        if (strcmp(command, "trans_mat") == 0) { /* if the command is trans_mat we should call the trans_mt method with the 2 matrices */
            if (!isalpha(*line)){
                /* ensure no extra text was entered. If so, skip to the next iteration of the while loop */
                CHECK_EXTRA_TEXT
            }

            trans_mat(*all[index], *all[second_index]);
            continue; /* we have executed the current command, we should continue to the next while loop iteration */
        }

        if (*line++ != ','){ /* ensure that there was a comma between the scalar and the result_matrix */
            three_mats_comma_problem = 1;
        }

        /* if we have skipped those five commands, it means we have at least 3 matrices one after the other, so we extract another matrix */

        read = get_token(line, result_matrix); /* read the third matrix and store it in its variable */
        third_index = result_matrix[MATRIX_NAME_LENGTH-1] - 'A'; /* extract the third matrix's index */

        line += read - 1;

        if (!legal_matrix(second_matrix, second_index) || !legal_matrix(result_matrix, third_index)){ /* verify the matrix is legal: legal and existing name */
            if (!strcmp(result_matrix, "")){
                /* if the matrix is none, it means there is a missing argument, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, Missing argument!\n");
            }
            else if (!strcmp(second_matrix, ",")){
                /* if the matrix is a comma, it means there were multiple consecutive commas, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, Multiple consecutive commas!\n");
            } else {
                /* if the matrix isn't legal, inform the user and continue to the next iteration of the while loop */
                printf("Sorry, undefined matrix name!\n");
            }
            continue; /* skip the further processing */
        }

        if (three_mats_comma_problem){ /* ensure that there was a comma between the scalar and the result_matrix */
            printf("Missing comma!\n"); /* inform the user */
            continue; /* skip the next processing and continue to the next iteration */
        }

        if (!isalpha(*line)){
            /* ensure no extra text was entered. If so, skip to the next iteration of the while loop */
            CHECK_EXTRA_TEXT
        }


        if (strcmp(command, "add_mat") == 0) { /* if the command is add_mat, execute the add_mat method with the 3 matrices we extracted */
            add_mat(*all[index], *all[second_index], *all[third_index]);
        } else if (strcmp(command, "sub_mat") == 0) { /* if the command is sub_mat, execute the sub_mat method with the 3 matrices we extracted */
            sub_mat(*all[index], *all[second_index], *all[third_index]);
        } else if (strcmp(command, "mul_mat") == 0) { /* iif the command is mul_mat, execute the mul_mat method with the 3 matrices we extracted */
            mul_mat(*all[index], *all[second_index], *all[third_index]);
        }
    }

    /* free the dynamically allocated variables */
    FREE_ALL
    printf("\nHave a Nice Day!\n");

    return 0;
}

