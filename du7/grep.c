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
_Bool find_pattern(char*, const char*, int*, int*, _Bool);
_Bool find_common(char*, const char*, int*, int*);
_Bool find_regular(char*, const char*, int*, int*);
void clean_lines(char***, int);


int main(int argc, const char *argv[]) {

  int ret = EXIT_SUCCESS; // variable for monitoring errors

  _Bool is_regular = FALSE;
  _Bool is_colored = FALSE;

  // const char *pattern = NULL;
  // const char *file_name = NULL;

  // DEBUG
  const char *pattern = "Mem";
  const char *file_name = "data/man/pub01-m.in";

  // Checking arguments
  // if (ret == EXIT_SUCCESS) {
  //   for (int i = 1; i < argc; i++) {
  //     if (my_strcmp(argv[i], "-E") && !pattern && !file_name)
  //       is_regular = TRUE;
  //     else if (my_strcmp(argv[i], "--color=always") && !pattern && !file_name)
  //       is_colored = TRUE;
  //     else if (!pattern)
  //       pattern = argv[i];
  //     else if (!file_name)
  //       file_name = argv[i];
  //     else {  // excess arguments
  //       ret = ERROR_ARGS;
  //       break;
  //     }
  //   }
  //   if (!pattern)
  //     ret = ERROR_ARGS;
  // }

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

  // Allocate memory for result
  *result = (char**)malloc(MAX_SIZE * sizeof(char*));
  if (!(*result))
    ret = ERROR_MEMORY;

  // Allocate memory for current line
  char *current_line = NULL;
  if (ret == EXIT_SUCCESS) {
    current_line = (char*)malloc(MAX_SIZE * sizeof(char));
    if (!current_line)
      ret = ERROR_MEMORY;
  }

  // Assign result
  if (ret == EXIT_SUCCESS) {
    while (fgets(current_line, MAX_SIZE, file)) {
      int a = 0, b = 0; // empty variables, just for sending in find_pattern
      if (find_pattern(current_line, pattern, &a, &b, is_regular)) {
        *result[*res_count] = current_line;
        printf("%s\n", *result[*res_count]);  // DEBUG
        printf("Pattern starts at %d and ends at %d\n", a, b);  // DEBUG
        (*res_count)++;
      }
    }
  }
  
  if (current_line)
    free(current_line);

  return ret;
}


_Bool find_pattern(char *line, const char *pattern, int *start, int *end, _Bool is_regular) {
  /* Decides, if line has pattern, returns start and end of pattern (indexes) */

  _Bool ret = FALSE;
  
  if(!is_regular)
    ret = find_common(line, pattern, start, end);
  else
    ret = find_regular(line, pattern, start, end);

  return ret;
}


_Bool find_common(char *line, const char *pattern, int *start, int *end) {
  /* Nonregular part of find_pattern function */

  int new_start = 0;  // variable to insert in 'start' before exit
  int matches = 0;  // number of matches with pattern
  int pat_len = my_strlen(pattern);
  int line_len = my_strlen(line);

  // Start searching from 'start'
  for (int i = *start; i < line_len; i++) {
    if (line[i] == pattern[0]) {
      new_start = i;
      matches++;
      i++;

      // Try to find all pattern's characters
      for (; i < line_len && matches != pat_len; i++) {
        if (line[i] == pattern[matches])
          matches++;
        else {
          matches = 0;
          break;
        }
      }

      // If pattern is found
      if (matches == pat_len) {
        *start = new_start;
        *end = i;
        return TRUE;
      }
    }
  }

  return FALSE;
}


_Bool find_regular(char *line, const char *pattern, int *start, int *end) {
  /* Regular part of find_pattern function */

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
