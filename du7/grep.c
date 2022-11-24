#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define MAX_SIZE 100


enum {
  EXIT_EMPTY = 1,
  ERROR_ARGS = 100,
  ERROR_FILE = 101,
  ERROR_MEMORY = 102
};


_Bool my_strcmp(const char*, const char*);
int my_strlen(const char*);
int alloc_and_fill(char***, char***, int*, FILE*);
int common_searching(FILE*, const char*, char***, int*, char**, int);
int regular_searching(FILE*, const char*, char***, int*, char**, int);
void clean_lines(char***, int);


int main(int argc, const char *argv[]) {

  int ret = EXIT_SUCCESS; // variable for monitoring errors

  _Bool is_regular = FALSE;
  _Bool is_colored = FALSE;

  const char *pattern = NULL;
  const char *file_name = NULL;

  // Checking arguments
  if (ret == EXIT_SUCCESS) {
    for (int i = 1; i < argc; i++) {
      if (my_strcmp(argv[i], "-E") && !pattern && !file_name)
        is_regular = TRUE;
      else if (my_strcmp(argv[i], "--color=always") && !pattern && !file_name)
        is_colored = TRUE;
      else if (!pattern)
        pattern = argv[i];
      else if (!file_name)
        file_name = argv[i];
      else {  // excess arguments
        ret = ERROR_ARGS;
        break;
      }
    }
    if (!pattern)
      ret = ERROR_ARGS;
  }

  // Open file
  FILE *file = NULL;
  if (ret == EXIT_SUCCESS) {
    file = (!file_name) ? stdin : fopen(file_name, "r");
    if (!file)
      ret = ERROR_FILE;
  }

  // Allocate memory for lines and result
  char **result = NULL;
  int res_count = 0;
  char **lines = NULL;
  int lines_count = 0;

  if (ret == EXIT_SUCCESS)
    ret = alloc_and_fill(&result, &lines, &lines_count, file);


  //TODO - check allocation and cleaning with valgrind

  // Searching
  if (ret == EXIT_SUCCESS)
    if (!is_regular)
      ret = common_searching(file, pattern, &result, &res_count, lines, lines_count);
    else
      ret = regular_searching(file, pattern, &result, &res_count, lines, lines_count);


  // TODO - after searching


  // Free allocated memory
  if (file)
    fclose(file);

  if (result)
    clean_lines(&result, res_count);

  if (lines)
    clean_lines(&lines, lines_count);
  
  // Checking of errors
  if (ret == ERROR_ARGS)
    fprintf(stderr, "Error: Invalid number of arguments!\n");
  else if (ret == ERROR_FILE)
    fprintf(stderr, "Error: File cannot be open!\n");
  else if (ret = ERROR_MEMORY)
    fprintf(stderr, "Error: Memory not allocated/reallocated!\n");

  return ret;
}


_Bool my_strcmp(const char *first, const char *second) {
  /* Returns TRUE, if strings are equal, else returns FALSE */

  int first_len = my_strlen(first);

  if (first_len != my_strlen(second))
    return FALSE;

  for (int i = 0; i < first_len; i++)
    if (first[i] != second[i])
      return FALSE;

  return TRUE;
}


int my_strlen(const char *string) {
  /* Analogue to function in string.h */
  
  int length = 0;
  for (int i = 0; string[i] != '\0'; i++)
    length++;
  
  return length;
}


int alloc_and_fill(char ***result, char ***lines, int *lines_count, FILE *file) {
  /* Allocates memory for result and lines. Also fills lines. */

  int ret = EXIT_SUCCESS;

  // Allocation
  *result = (char**)malloc(MAX_SIZE * sizeof(char*));
  *lines = (char**)malloc(MAX_SIZE * sizeof(char*));
  if (!(*result) || !(*lines))
    ret = ERROR_MEMORY;

  // Read all lines in the file
  if (ret == EXIT_SUCCESS)
    for (int i = 0; fgets((*lines)[i], MAX_SIZE, file); i++)
      (*lines_count)++;
  
  // Reduce lines
  if (ret == EXIT_SUCCESS) {
    char **tmp = (char**)realloc(*lines, (*lines_count) * sizeof(char*));
    if (!tmp)
      ret = ERROR_MEMORY;
    
    if (ret == EXIT_SUCCESS)
      *lines = tmp;
  }

  return ret;
}


int common_searching(FILE *file, const char *pattern, char ***result, int *res_count, char **lines, int lines_count) {
  // TODO - common searching
}


int regular_searching(FILE *file, const char *pattern, char ***result, int *res_count, char **lines, int lines_count) {
  // TODO - regular searching
}


void clean_lines(char ***lines, int count) {
  /* Cleans array of strings */

  if (!lines || !(*lines))
    return;

  for (int i = 0; i < count; i++)
    free((*lines)[i]);

  *lines = NULL;
}
