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


void read_input(Matrix**, char**, int*, int*, int*);
void clean_matrices(Matrix**, int);
void clean_operators(char**);
void read_sizes(Matrix*, int*);
void allocate_values(Matrix*, int*);
void clean_matrix(Matrix*, int);
void read_values(Matrix*, int*);


int main(int argc, char *argv[])
{
  int ret = EXIT_SUCCESS; // variable for monitoring errors

  int mat_count = 0;  // number of matrices
  Matrix *matrices = NULL;  // array of matrices

  int oper_count = 0; // number of operators
  char *operators = NULL; // array of operators

  read_input(&matrices, &operators, &mat_count, &oper_count, &ret);

  //TODO

  // Cleaning memory (matrices and operators)
  if (matrices)
    clean_matrices(&matrices, mat_count);

  if (operators)
    clean_operators(&operators);

  // Cheking of errors
  if (ret == ERROR_INPUT)
    fprintf(stderr, "Error: Chybny vstup!\n");
  else if (ret == ERROR_MEMORY)
    fprintf(stderr, "Error: Neprobehla alokace/realokace!\n");

  return ret;
}


void read_input(Matrix **mtrxs, char **opers, int *mat_count, int *oper_count, int *ret) {

  *mtrxs = (Matrix*)malloc(sizeof(Matrix) * MAX_LENGTH);
  if (!(*mtrxs)) {
    *ret = ERROR_MEMORY;
    return;
  }

  *opers = (char*)malloc(sizeof(char) * MAX_LENGTH);
  if (!(*opers)) {
    *ret = ERROR_MEMORY;
    return;
  }

  while (TRUE) {
    read_sizes(&(*mtrxs)[*mat_count], ret);
    if (*ret != EXIT_SUCCESS) {
      clean_matrices(mtrxs, *mat_count);
      clean_operators(opers);
      return;
    }

    allocate_values(&(*mtrxs)[*mat_count], ret);
    if (*ret != EXIT_SUCCESS) {
      clean_matrices(mtrxs, *mat_count);
      clean_operators(opers);
      return;
    }

    read_values(&(*mtrxs)[*mat_count], ret);
    if (*ret != EXIT_SUCCESS) {
      clean_matrices(mtrxs, *mat_count);
      clean_operators(opers);
      return;
    }

    (*mat_count)++;

    getchar();
    char operator;
    if (scanf("%c", &operator) == EOF)
      break;

    if (operator == ADD || operator == SUB || operator == MUL) {
      (*opers)[*oper_count] = operator;
      (*oper_count)++;
    }
    else {
      *ret = ERROR_INPUT;
      clean_matrices(mtrxs, *mat_count);
      clean_operators(opers);
      return;
    }
  }

  Matrix *tmp_mat = realloc(*mtrxs, sizeof(Matrix) * (*mat_count));
  if (!tmp_mat) {
    *ret = ERROR_MEMORY;
    clean_matrices(mtrxs, *mat_count);
    clean_operators(opers);
    return;
  }
  *mtrxs = tmp_mat;

  char *tmp_oper = realloc(*opers, sizeof(char) * (*oper_count));
  if (!tmp_oper) {
    *ret = ERROR_MEMORY;
    clean_matrices(mtrxs, *mat_count);
    clean_operators(opers);
    return;
  }
  *opers = tmp_oper;
}


void clean_operators(char **operators) {
  if (!operators)
    return;

  free(*operators);
  *operators = NULL;
}


void clean_matrices(Matrix **matrices, int count) {
  if (!matrices && !(*matrices))
    return;

  for (int i = 0; i < count; i++)
    clean_matrix(&(*matrices)[i], (*matrices)[i].rows);

  free(*matrices);
  *matrices = NULL;
}


void clean_matrix(Matrix* matrix, int rows) {
/* Function cleans values of the matrix */

  if (!matrix->values)
    return;

  for (int i = 0; i < rows; i++)
    free(matrix->values[i]);
  
  free(matrix->values);
  matrix->values = NULL;
}


void read_sizes(Matrix *matrix, int *ret) {
  /* Function reads sizes of the matrix */

  if (scanf("%d", &matrix->rows) != 1 || scanf("%d", &matrix->columns) != 1)
    *ret = ERROR_INPUT;
}


void allocate_values(Matrix *matrix, int *ret) {
  /* Function allocaes memory for values of the matrix */

  matrix->values = (int**)malloc(sizeof(int*) * matrix->rows);
  
  if(!matrix->values) {
    *ret = ERROR_MEMORY;
    return;
  }

  for (int i = 0; i < matrix->rows; i++) {
    matrix->values[i] = (int*)malloc(sizeof(int) * matrix->columns);
    if(!matrix->values[i]) {
      *ret = ERROR_MEMORY;
      clean_matrix(matrix, i);
      return;
    }
  }
}


void read_values(Matrix* matrix, int* ret) {
/* Function reads values of the matrix */

  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      if (scanf("%d", &matrix->values[i][j]) != 1) {
        *ret = ERROR_INPUT;
        clean_matrix(matrix, matrix->rows);
        return;
      }
    }
  }
}
