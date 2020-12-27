/* size of the matrix */
#define SIZE 4

#include <stdio.h>

typedef double mat[SIZE][SIZE];

/* Input: receives a matrix, an array of doubles and its length.
 * Output: returns nothing. fills the matrix with the values obtained from the array.
 *
 * This function receives a matrix alongside an array of values with its length, and fills the matrix with the received values*/
void read_mat(mat matrix, double vals[], int length);

/* Input: a matrix.
 * Output: returns nothing. Prints the matrix to stdin.
 *
 * This function receives a matrix and prints it to stdin, with proper formatting and indentation */
void print_mat(mat matrix);

/* Input: receives three matrices.
 * Output: returns nothing. Adds mat1, mat2 together and stores the result in mat3.
 *
 * This function performs addition on the first 2 matrices and stores the result in mat3 */
void add_mat(mat mat1, mat mat2, mat mat3);

/* Input: receives three matrices.
 * Output: returns nothing. Subtracts mat2 from mat1 and stores the result in mat3.
 *
 * This function performs subtraction on the first 2 matrices and stores the result in mat3 */
void sub_mat(mat mat1, mat mat2, mat mat3);

/* Input: receives three matrices.
 * Output: returns nothing. Performs matrix multiplication on mat1, mat3 and stores the result in mat3.
 *
 * This function performs matrix multiplication on the first 2 matrices and stores the result in mat3 */
void mul_mat(mat mat1, mat mat2, mat mat3);

/* Input: receives two matrices and a scalar.
 * Output: returns nothing. Multiplies mat1 by the scalar and stores the result in mat2.
 *
 * This function receives 2 matrices and a scalar, multiply the first matrix by the scalar and stores the result in the second*/
void mul_scalar(mat mat1, double val, mat mat2);

/* Input: receives two matrices.
 * Output: returns nothing. Transposes mat1 and stores the result in mat2.
 *
 * This function receives 2 matrices, transposes the first matrix and stores the result in the second */
void trans_mat(mat mat1, mat mat2);

/* Input: a matrix.
 * Output: returns nothing. Sets the value of all cells to 0.
 *
 * This function receives a matrix sets the value of all its cells to 0 */
void reset_mat(mat matrix);

/* Input: receives two matrices.
 * Output: returns nothing. Copies the content of mat1 to mat2.
 *
 * This function receives 2 matrices and copies the content of the first (src) to the second (dest) */
void copy_mat(mat src, mat dest);

/* Input: row index, column index, two matrices
 * Output: returns the dot-product of the {row} from mat1 and the {col} from mat2
 *
 * This function receives 2 matrices as well as row and column indexes, performs a dot-product on the row and column, and returns the result */
double scalar_mult(int row, int col, mat mat1, mat mat2); /* returns the dot product of a row and a column */




