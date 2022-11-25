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
int searching(char***, int*, const char*, FILE*, _Bool);
_Bool find_common(char*, const char*, int*, int*);
_Bool find_regular(char*, const char*, int*, int*);
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

  int res_count = 0;
  char** result = NULL;

  // Searching
  if (ret == EXIT_SUCCESS)
    ret = searching(&result, &res_count, pattern, file, is_regular);


  // TODO - after searching


  // Free allocated memory
  if (file)
    fclose(file);

  if (result)
    clean_lines(&result, res_count);
  
  // Checking of errors
  if (ret == ERROR_ARGS)
    fprintf(stderr, "Error: Invalid number of arguments!\n");
  else if (ret == ERROR_FILE)
    fprintf(stderr, "Error: File cannot be open!\n");
  else if (ret == ERROR_MEMORY)
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


int searching(char ***result, int *res_count, const char *pattern, FILE *file, _Bool is_regular) {
  /* Function finds lines with pattern */

  //TODO - check this algorithm

  int ret = EXIT_SUCCESS;

  *result = (char**)malloc(MAX_SIZE * sizeof(char*));
  if (!(*result))
    ret = ERROR_MEMORY;

  // Find paterns
  if (ret == EXIT_SUCCESS) {
    char *current_line = (char*)malloc(MAX_SIZE * sizeof(char));
    while (fgets(current_line, MAX_SIZE, file)) {
      int a = 0, b = 0; // empty variables, just for find_pattern
      if ((!is_regular && find_common(current_line, pattern, &a, &b)) || find_regular(current_line, pattern, &a, &b)) {
        *result[*res_count] = current_line;
        (*res_count)++;
      }
      else
        free(current_line);
    }
  }

  return ret;
}


_Bool find_common(char *line, const char *pattern, int *start, int *end) {
  /* Decides, is line has pattern. Starts searching from start.
    Return start and end of pattern (indexes). */

  //TODO - check this algorithm

  int pat_len = my_strlen(pattern);

  for (int i = *start; line[i] != '\0'; i++) {
    if (line[i] == pattern[0]) {
      int matches = 1;
      int new_start = i;
      i++;
      for (; ; i++) {
        if (matches == pat_len) {
          *start = new_start;
          *end = i - 1;
          return TRUE;
        }

        if (line[i] == '\0')
          return FALSE;

        if (line[i] == pattern[matches])
          matches++;
        else
          break;
      }
    }
  }

  return FALSE;
}


_Bool find_regular(char *line, const char *pattern, int *start, int *end) {
  

  //TODO - write this algorithm


  return FALSE;
}


void clean_lines(char ***lines, int count) {
  /* Cleans array of strings */

  if (!lines || !(*lines))
    return;

  for (int i = 0; i < count; i++)
    free(*lines[i]);

  free(*lines);
  *lines = NULL;
}
