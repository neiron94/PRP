#include <stdio.h>
#include <stdlib.h>


#define MAX_LENGTH 100 // max possible length of array of matrices and operators
#define TRUE 1
#define ADD '+'
#define SUB '-'
#define MUL '*'


enum {
  ERROR_INPUT = 100,
  ERROR_MEMORY = 101
};


typedef struct Matrix {
  int **values; // matrix of values of Matrix
  int rows;
  int columns;
} Matrix;


int read_input(Matrix**, char**, int*, int*);
int read_sizes(Matrix*);
int allocate_values(Matrix*);
int read_values(Matrix*);
int reduce(Matrix**, char**, int*, int*);
void clean_matrices(Matrix**, int);
void clean_matrix(Matrix*, int);
void clean_operators(char**);
int calculate(Matrix*, Matrix*, char*, int);
void matrix_multiply(Matrix, Matrix, Matrix*);
void matrix_add(Matrix*, Matrix);
void matrix_subtraction(Matrix*, Matrix);
void move_matrices(Matrix*, int, int);
void move_operators(char*, int, int);
void print_matrix(Matrix);


int main(int argc, char *argv[])
{
  int ret = EXIT_SUCCESS; // variable for monitoring errors

  int mat_count = 0;  // number of matrices
  Matrix *matrices = NULL;  // array of matrices

  int oper_count = 0; // number of operators
  char *operators = NULL; // array of operators

  ret = read_input(&matrices, &operators, &mat_count, &oper_count);

  Matrix result;
  result.values = NULL;
  if (ret == EXIT_SUCCESS)
    ret = calculate(&result, matrices, operators, oper_count);

  if (ret == EXIT_SUCCESS)
    print_matrix(result);

  // Cleaning memory (matrices and operators)
  if (matrices)
    clean_matrices(&matrices, mat_count);

  if (result.values)
    clean_matrix(&result, result.rows);

  if (operators)
    clean_operators(&operators);

  // Cheking of errors
  if (ret == ERROR_INPUT)
    fprintf(stderr, "Error: Chybny vstup!\n");
  else if (ret == ERROR_MEMORY)
    fprintf(stderr, "Error: Neprobehla alokace/realokace!\n");

  return ret;
}


int read_input(Matrix **mtrxs, char **opers, int *mat_count, int *oper_count) {
  /* Reads all input (matrices and operators) */

  int ret = EXIT_SUCCESS; // variable for monitoring errors

  // Allocate arrays of matrices and operators to MAX_LENGTH
  *mtrxs = (Matrix*)malloc(sizeof(Matrix) * MAX_LENGTH);
  // Checking of a successful allocation
  if (!(*mtrxs))
    ret = ERROR_MEMORY;

  if (ret == EXIT_SUCCESS) {
    *opers = (char*)malloc(sizeof(char) * MAX_LENGTH);
    // Checking of a successful allocation
    if (!(*opers))
      ret = ERROR_MEMORY;
  }

  // Reading input
  if (ret == EXIT_SUCCESS) {
    while (TRUE) {

      // Filling current matrix
      Matrix *current_matrix = &(*mtrxs)[*mat_count]; // current matrix to fill

      ret = read_sizes(current_matrix);
      if (ret != EXIT_SUCCESS)
        break;

      ret = allocate_values(current_matrix);
      if (ret != EXIT_SUCCESS)
        break;

      ret = read_values(current_matrix);
      if (ret != EXIT_SUCCESS)
        break;

      (*mat_count)++;

      // Reading current operator (or exit the loop)
      char operator;
      if (scanf(" %c", &operator) == EOF)  // exit the loop, if EOF
        break;

      if (operator == ADD || operator == SUB || operator == MUL) {
        (*opers)[*oper_count] = operator;
        (*oper_count)++;
      }
      else {
        // Neither '+', nor '-', nor '*'
        ret = ERROR_INPUT;
        break;
      }
    }
  }

  if (ret == EXIT_SUCCESS)
    ret = reduce(mtrxs, opers, mat_count, oper_count);

  if (ret != EXIT_SUCCESS) {
    clean_matrices(mtrxs, *mat_count);
    clean_operators(opers);
  }

  return ret;
}


int read_sizes(Matrix *matrix) {
  /* Reads sizes of the matrix */

  if (scanf("%d", &matrix->rows) != 1 || scanf("%d", &matrix->columns) != 1)
    return ERROR_INPUT;

  return EXIT_SUCCESS;
}


int allocate_values(Matrix *matrix) {
  /* Allocates memory for values of the matrix */

  matrix->values = (int**)malloc(sizeof(int*) * matrix->rows);
  // Checking of a successful allocation
  if(!matrix->values)
    return ERROR_MEMORY;

  for (int i = 0; i < matrix->rows; i++) {
    matrix->values[i] = (int*)malloc(sizeof(int) * matrix->columns);
    // Checking of a successful allocation
    if(!matrix->values[i]) {
      clean_matrix(matrix, i);
      return ERROR_MEMORY;
    }
  }

  return EXIT_SUCCESS;
}


int read_values(Matrix* matrix) {
  /* Reads values of the matrix */

  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      if (scanf("%d", &matrix->values[i][j]) != 1) {
        clean_matrix(matrix, matrix->rows);
        return ERROR_INPUT;
      }
    }
  }

  return EXIT_SUCCESS;
}


int reduce(Matrix **mtrxs, char **opers, int *mat_count, int *oper_count) {
  /* Reduces arrays of matrices and operators */

  Matrix *tmp_mat = realloc(*mtrxs, sizeof(Matrix) * (*mat_count));
  // Checking of a successful reallocation
  if (!tmp_mat)
   return ERROR_MEMORY;
 
  *mtrxs = tmp_mat;
  
  char *tmp_oper = realloc(*opers, sizeof(char) * (*oper_count));
  // Checking of a successful allocation
  if (!tmp_oper)
    return ERROR_MEMORY;

  *opers = tmp_oper;

  return EXIT_SUCCESS;
}


void clean_matrices(Matrix **matrices, int count) {
  /* Cleans array of matrices and sets pointer to NULL */

  if (!matrices || !(*matrices))
    return;

  for (int i = 0; i < count; i++)
    clean_matrix(&(*matrices)[i], (*matrices)[i].rows);

  free(*matrices);
  *matrices = NULL;
}


void clean_matrix(Matrix* matrix, int rows) {
  /* Cleans values of the matrix and sets pointer to NULL */

  if (!matrix || !matrix->values)
    return;

  for (int i = 0; i < rows; i++) {
    free(matrix->values[i]);
  }
  
  free(matrix->values);
  matrix->values = NULL;
}


void clean_operators(char **operators) {
  /* Cleans array of operators and sets pointer to NULL */

  if (!operators || !(*operators))
    return;

  free(*operators);
  *operators = NULL;
}


int calculate(Matrix *result, Matrix *mtrxs, char *opers, int oper_count) {
  /* Goes through array of operators twice. Firstly does all
     multiplications, then all other operations. */

  int ret = EXIT_SUCCESS;

  for (int i = 0; i < oper_count; i++) {
    if (opers[i] == MUL) {

      if (mtrxs[i].columns != mtrxs[i + 1].rows)
        return ERROR_INPUT;
      
      Matrix tmp_matrix;
      tmp_matrix.rows = mtrxs[i].rows;
      tmp_matrix.columns = mtrxs[i + 1].columns;
      ret = allocate_values(&tmp_matrix);
      if (ret != EXIT_SUCCESS)
        return ret;
      matrix_multiply(mtrxs[i], mtrxs[i + 1], &tmp_matrix);

      clean_matrix(&mtrxs[i], mtrxs[i].rows);
      clean_matrix(&mtrxs[i + 1], mtrxs[i + 1].rows);

      move_matrices(mtrxs, i + 1, oper_count);
      mtrxs[i] = tmp_matrix;
      move_operators(opers, i + 1, oper_count - 1);

      oper_count--;
      i--;
    }
  }

  result->rows = mtrxs[0].rows;
  result->columns = mtrxs[0].columns;
  ret = allocate_values(result);
  if (ret != EXIT_SUCCESS)
    return ret;
  for (int i = 0; i < result->rows; i++)
    for (int j = 0; j < result->columns; j++)
      result->values[i][j] = mtrxs[0].values[i][j];

  for (int i = 0; i < oper_count; i++) {
    if (result->rows != mtrxs[i + 1].rows || result->columns != mtrxs[i + 1].columns)
      return ERROR_INPUT;

    if (opers[i] == ADD)
      matrix_add(result, mtrxs[i + 1]);
    else 
      matrix_subtraction(result, mtrxs[i + 1]);
  }

  return EXIT_SUCCESS;
}


void matrix_multiply(Matrix left, Matrix right, Matrix *result) {

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      result->values[i][j] = 0;
      for (int n = 0; n < left.columns; n++) {
        result->values[i][j] += left.values[i][n] * right.values[n][j];
      }
    }
  }
}


void matrix_add(Matrix *left, Matrix right) {

  for (int i = 0; i < left->rows; i++)
    for (int j = 0; j < left->columns; j++)
      left->values[i][j] += right.values[i][j];
}


void matrix_subtraction(Matrix *left, Matrix right) {

  for (int i = 0; i < left->rows; i++)
    for (int j = 0; j < left->columns; j++)
      left->values[i][j] -= right.values[i][j];
}


void move_matrices(Matrix *mtrxs, int start_i, int end_i) {

  for (int i = start_i; i <= end_i; i++) {
    mtrxs[i - 1] = mtrxs[i];
    mtrxs[i].values = NULL;
  }
}


void move_operators(char *opers, int start_i, int end_i) {

  for (int i = start_i; i <= end_i; i++)
    opers[i - 1] = opers[i];
}


void print_matrix(Matrix matrix) {
  /* Prints one matrix */

  printf("%d %d\n", matrix.rows, matrix.columns);

  for (int i = 0; i < matrix.rows; i++) {
    for (int j = 0; j < matrix.columns; j++) {
      if (j != 0)
        printf(" ");
      
      printf("%d", matrix.values[i][j]);
    }
    printf("\n");
  }
}
