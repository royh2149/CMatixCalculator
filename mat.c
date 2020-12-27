#include "mat.h"

#define GO_OVER_ROWS for (i = 0; i < SIZE; i++)
#define GO_OVER_COLUMNS for (j = 0; j < SIZE; j++)

/* Input: receives a matrix, an array of doubles and its length.
 * Output: returns nothing. fills the matrix with the values obtained from the array.
 *
 * Algorithm: start from the upper-left cell. Iterate through the received array, and fill the matrix with the values.
 * After each assignment, check whether the current row is finished - if all the columns in it were already dealt with.
 * If so, proceed to the next line and reset the column variable to 0. If not, increment column. */
void read_mat(mat matrix, double vals[], int length){
    int row = 0, col = 0, i = 0; /* start at the first cell of the matrix */

    for(i = 0; i < length; i++){ /* iterate through the values array */
        matrix[row][col] = vals[i]; /* set current cell value to the current value in the received array */
        if (++col == SIZE){ /* it means the current row is finished */
            row++; /* proceed to the next line */
            col = 0; /* begin from the most-left column in the new line */
        } else{
            /* proceed to the next column, we have already updated col in the if */
        }

        if (row >= SIZE){ /* prevent overflow to other matrices - if too many values were provided, ignore them */
            break;
        }
    }
}

/* Input: a matrix.
 * Output: returns nothing. Prints the matrix to stdin.
 *
 * Algorithm: iterate through the rows. In each iteration:
 *              1. iterate through all the columns and print the cells' values with 7 chars indentation and 2 digits after the dot.
 *              2. when all the values of the row are printed, print a new line. */
void print_mat(mat matrix){
    int i, j; /* index variables to iterate over the matrix */

    printf("\n"); /* print a blank line so that the output would be clear */
    GO_OVER_ROWS { /* iterate through the rows */
        GO_OVER_COLUMNS { /* iterate through the columns */
            printf("%7.2f    ", matrix[i][j]); /* print the value of the cell */
        }

        printf("\n"); /* write each row in a new line */
    }
}

/* Input: receives three matrices.
 * Output: returns nothing. Adds mat1, mat2 together and stores the result in mat3.
 *
 * Algorithm: iterate through the cells of mat3 - the result matrix.
 *            Set each cell's value to the sum of the corresponding cells of mat1, mat2. */
void add_mat(mat mat1, mat mat2, mat mat3){
    int i, j; /* index variables to iterate over the matrices */

    GO_OVER_ROWS { /* iterate through the rows */
        GO_OVER_COLUMNS { /* iterate through the columns */
            mat3[i][j] = mat1[i][j] + mat2[i][j]; /* store the addition result of the 2 cells in the appropriate cell */
        }
    }
}

/* Input: receives three matrices.
 * Output: returns nothing. Subtracts mat2 from mat1 and stores the result in mat3.
 *
 * Algorithm: iterate through the cells of mat3 - the result matrix.
 *            Set each cell's value to the result of the subtraction of the corresponding cells of mat1, mat2. */
void sub_mat(mat mat1, mat mat2, mat mat3){
    int i, j; /* index variables to iterate over the matrices */

    GO_OVER_ROWS { /* iterate through the rows */
        GO_OVER_COLUMNS { /* iterate through the columns */
            mat3[i][j] = mat1[i][j] - mat2[i][j]; /* store the addition result of the 2 cells in the appropriate cell */
        }
    }
}

/* Input: receives three matrices.
 * Output: returns nothing. Performs matrix multiplication on mat1, mat3 and stores the result in mat3.
 *
 * Algorithm: first, we create a temporary matrix to hold the result in. That's because if mat3 is also either mat1 or mat2,
 * the multiplication will fail. Then, we iterate over the cells of the result matrix, and set each one's value to the result
 * of the dot product operation of {i} row of mat1 and {j} column of mat2. At the end, we copy the contents of the result
 * matrix to mat3. */
void mul_mat(mat mat1, mat mat2, mat mat3){
    int i, j; /* index variables to iterate over the matrices */
    mat result; /* create a matrix to hold a result. If a matrix is both mat1 or mat2 and mat3 as well, not using a temporary matrix might cause mistakes */

    GO_OVER_ROWS { /* iterate through the rows */
        GO_OVER_COLUMNS { /* iterate through the columns */
            result[i][j] = scalar_mult(i, j, mat1, mat2); /* store the scalar multiplication result of the current row-col pair in the appropriate cell */
        }
    }

    copy_mat(result, mat3); /* copy the result matrix to the received result matrix */
}

/* Input: receives two matrices and a scalar.
 * Output: returns nothing. Multiplies mat1 by the scalar and stores the result in mat2.
 *
 * Algorithm: iterate through all the cells of mat2. Set each cell's value to the value of the corresponding cell in mat1,
 * times the scalar. */
void mul_scalar(mat mat1, double val, mat mat2){
    int i, j; /* index variables to iterate over the matrices */

    GO_OVER_ROWS { /* iterate through the rows */
        GO_OVER_COLUMNS { /* iterate through the columns */
            mat2[i][j] = mat1[i][j] * val; /* store the multiplication result of the current cell and the scalar in the appropriate cell */
        }
    }
}

/* Input: receives two matrices.
 * Output: returns nothing. Transposes mat1 and stores the result in mat2.
 *
 * Algorithm: first, we create a temporary matrix to hold the result in. That's because if mat1 and mat2 are the same,
 * the transposition will fail. Then, we iterate over all the cells and copy their content to the corresponding cell
 * in the result matrix: the symmetric cell (cell[i][j] --> cell[j][i]). At the end, we copy the contents of the result
 * matrix to mat2. */
void trans_mat(mat mat1, mat mat2){
    int i, j; /* index variables to iterate over the matrices */
    mat result; /* create a matrix to hold a result. If a matrix is both mat1 and mat2, not using a temporary matrix might cause mistakes */

    GO_OVER_ROWS { /* iterate through the rows */
        GO_OVER_COLUMNS { /* iterate through the columns */
            result[i][j] = mat1[j][i]; /* store the transposed value of the current cell appropriate cell */
        }
    }

    copy_mat(result, mat2); /* copy the result matrix to the received result matrix */
}

/* Input: row index, column index, two matrices
 * Output: returns the dot-product of the {row} from mat1 and the {col} from mat2
 *
 * Algorithm: create a variable to hold the result and a variable for the for loop. Reset both to 0.
 *            Run the loop SIZE times. In each iteration, add to the result variable the multiplication of the cells in:
 *              1. K column in the {row} row of mat1
 *              2. {col} column in the K row of mat2 */
double scalar_mult(int row, int col, mat mat1, mat mat2){
    double result = 0;
    int k;

    for (k = 0;  k < SIZE; k++) { /* traverse through each pair */
        result += mat1[row][k] * mat2[k][col]; /* add the multiplication of the current pair to the result */
    }

    return result; /* return the scalar multiplication */
}

/* Input: a matrix.
 * Output: returns nothing. Sets the value of all cells to 0.
 *
 * Algorithm: iterate through the cells of the matrix. Set each one's value to 0. */
void reset_mat(mat matrix){
    int i,j; /* index variables to iterate over the matrix */

    GO_OVER_ROWS { /* iterate through the rows */
        GO_OVER_COLUMNS { /* iterate through the columns */
            matrix[i][j] = 0; /* change the value of the cell to 0 */
        }
    }
}

/* Input: receives two matrices.
 * Output: returns nothing. Copies the content of mat1 to mat2.
 *
 * Algorithm: iterate through the cells of the destination matrix. Set each one's value to the value of the corresponding cell in the source matrix.. */
void copy_mat(mat src, mat dest){
    int i,j; /* index variables to iterate over the matrices */

    GO_OVER_ROWS { /* iterate through the rows */
        GO_OVER_COLUMNS { /* iterate through the columns */
            dest[i][j] = src[i][j]; /* copy the value from the source cell to the destination cell */
        }
    }
}

