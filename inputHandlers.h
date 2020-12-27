#include "mat.h"

/* max length of a scalar */
#define MAX_NUM_LENGTH 100

/* max buffer size: full matrix arguments and more space for commas, command, etc */
#define MAX_BUF SIZE * SIZE * MAX_NUM_LENGTH + MAX_NUM_LENGTH

/* max length of a command - strlen("mul_scalar") */
#define MAX_COMMAND_LENGTH 10

#define MATRIX_NAME_LENGTH 5
#define NUM_OF_MATRICES 6
#define COMMANDS "stop trans_mat mul_scalar add_mat sub_mat read_mat print_mat mul_mat"

/* error codes */
#define EOF_ENCOUNTERED_ERROR 1
#define MEMORY_ALLOC_ERROR 2
#define FILE_ERROR 3

/* Input: receives 2 strings, one representing a line of text, and the other is a destination string
 * Output: returns the amount of chars read.
 *         Extracts the next token (from the first no-space char until a comma or space is reached) and stores it in the destination string
 *
 * This function receives a line of text and a destination char pointer (dest), and stores in dest the next token as explained earlier.
 * Returns the amount of chars read.
 * */
int get_token(char *line, char *dest);

/* Input: receives a string that represents name of a matrix and an int representing its supposed index in matrices array.
 * Output: returns 0 if the matrix is not legal for our purposes, non-zero value otherwise.
 *
 * This function receives a name of matrix and its supposed index in the matrices array and checks whether it is a legal one.
 * A legal matrix is of the pattern "MAT_X" whereas X is one of ABCDEF, whose index is from 0 to (NUM_OF_MATRICES - 1).
 * */
int legal_matrix(char *matrix, int index); 

/* Input: receives a string that represents a command.
 * Output: returns 0 if the command isn't legal, non-zero value otherwise.
 *
 * This function receives a command and return 0 if it isn't legal. Else, a non-zero value is returned.
 * A legal command is one of the specified above, not an empty string and not a space. */
int legal_command(char *command);


/* Input: receives a string that (supposedly) represents a scalar.
 * Output: returns 0 if the scalar isn't a valid one, returns 1 otherwise.
 *
 * This function receives a scalar as a string, and returns 1 if it stands for a valid scalar. If not, 0 is returned.
 * A valid scalar might have a sign (+ or -), may begin with a dot, has only one dot, and except these contains digits only.
 */
int legal_scalar(char *scalar); 

/* Input: a string as a character pointer
 * Output: returns 0 if there is text other than white chars, 1 if only white chars
 *
 * This function checks for extraneous text from the character pointer received to the end.
 * It will return 0 to represent "false", if there was an extraneous text, and 1 if not.
 *
 * */
int no_extra_text(char *str);

/* Input: a string representing a filename
 * Output: returns nothing. Prints the file to the screen.
 *
 * This function receives a name of a file and prints its content to the screen.
 */
void print_file(char *filename);
