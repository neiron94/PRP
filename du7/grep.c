#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define START_SIZE 100
#define ESC 27
#define INVALID_INDEX -1


enum {
  EXIT_EMPTY = 1,
  ERROR_ARGS = 100,
  ERROR_FILE = 101,
  ERROR_MEMORY = 102
};


_Bool my_strcmp(const char*, const char*);
int my_strlen(const char*);
char** read_file(FILE*, int*, int*);
char* read_line(FILE*, int*);
char** searching(int*, int*, char**, int, const char*, _Bool);
_Bool find_pattern(char*, const char*, int*, int*, _Bool);
_Bool find_common(char*, const char*, int*, int*);
_Bool find_regular(char*, const char*, int*, int*);
void reg_exp(char*, char, char, int*, int*, int*, int*);
_Bool is_expression(char);
void print_result(char**, int, const char*, _Bool, _Bool);
void print_colored(char*, const char*, _Bool);
void clean_lines(char**, int);


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

  // Read lines from file and close it
  char** lines = NULL;
  int lines_count = 0;
  lines = read_file(file, &lines_count, &ret);

  if (file)
    fclose(file);

  // Searching
  int res_count = 0;
  char** result = NULL;
  
  if (ret == EXIT_SUCCESS)
    result = searching(&ret, &res_count, lines, lines_count, pattern, is_regular);

  // Free allocated memory
  if (lines) {
    clean_lines(lines, lines_count);
    lines = NULL;
  }

  // Case of empty result
  if (ret == EXIT_SUCCESS && res_count == 0)
    ret = EXIT_EMPTY;

  // Print result
  if (ret == EXIT_SUCCESS)
    print_result(result, res_count, pattern, is_colored, is_regular);

  // Free allocated memory
  if (result) {
    clean_lines(result, res_count);
    result = NULL;
  }
  
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


char** read_file(FILE *file, int *count, int *ret) {
  /* Reads all lines from file */

    int size = START_SIZE;

    char **lines = (char**)malloc(size * sizeof(char*));
    if (!lines)
      *ret = ERROR_MEMORY;

    if (*ret == EXIT_SUCCESS) {
      while(!feof(file)) {

        // Increase allocated memory
        if (*count == size) {
          size *= 2;
          char **tmp = realloc(lines, size * sizeof(char*));
          if (!tmp) {
            *ret = ERROR_MEMORY;
            break;
          }
          lines = tmp;
          
        }

        lines[*count] = read_line(file, ret);
        if (*ret != EXIT_SUCCESS)
          break;

        (*count)++;
      }
    }

    // Reduce amount of lines
    char **tmp = realloc(lines, (*count) * sizeof(char*));
    if (!tmp)
      *ret = ERROR_MEMORY;
    lines = tmp;

    return lines;
}


char* read_line(FILE *file, int *ret) {
  /* Reads one line from file */

  int size = START_SIZE;
  int count = 0;

  char *line = (char*)malloc(size * sizeof(char));

  while (!feof(file)) {

    // Increase size of line
    if (count == size) {
      size *= 2;
      char *tmp = realloc(line, size * sizeof(char));
      if (!tmp) {
        free(line);
        *ret = ERROR_MEMORY;
        break;
      }
      line = tmp;
    }

    line[count] = fgetc(file);
    count++;
    if (line[count - 1] == '\n')
      break;
  }

  // Reduce size of line
  char *tmp = realloc(line, (count + 1) * sizeof(char));
  if (!tmp) {
    free(line);
    *ret = ERROR_MEMORY;
  }
  line = tmp;

  line[count] = '\0';

  return line;
}


char** searching(int *ret, int *res_count, char **lines, int lines_count, const char *pattern, _Bool is_regular) {
  /* Function finds lines with pattern */

  // Allocate memory for result
  char **result = (char**)malloc(START_SIZE * sizeof(char*));
  if (!result)
    *ret = ERROR_MEMORY;

  if (*ret == EXIT_SUCCESS) {
    for (int i = 0; i < lines_count; i++) {

      // Assign result
      int a = 0, b = 0; // empty variables, just for sending in find_pattern
      if (find_pattern(lines[i], pattern, &a, &b, is_regular)) {
        result[*res_count] = lines[i];
        lines[i] = NULL;
        (*res_count)++;
      }
    }
  }

  return result;
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

  int new_start = INVALID_INDEX;  // variable to insert in 'start' before exit
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

  int len_line = my_strlen(line);
  int len_pat = my_strlen(pattern);
  int new_start = INVALID_INDEX;
  int new_end = INVALID_INDEX;

  for (int i = *start, j = 0; i < len_line && j <= len_pat; i++) {
    if (j == len_pat) {
      *start = new_start;
      *end = new_end;
      return TRUE;
    }

    if (is_expression(pattern[j])) {
      j++;
      i--;
    }
    else if (j != len_pat - 1 && is_expression(pattern[j + 1])) {
      reg_exp(line, pattern[j], pattern[j + 1], &i, &j, &new_start, &new_end);
    }
    else if (line[i] == pattern[j]) {
      if (new_start == INVALID_INDEX) {
        new_start = i;
        new_end = i + 1;
      }
      else {
        new_end++;
      }
      j++;
    }
    else {
      j = 0;
      new_start = INVALID_INDEX;
      new_end = INVALID_INDEX;
    }
  }

  return FALSE;
}


void reg_exp(char *line, char ch, char exp, int *i, int *j, int *new_start, int *new_end) {
  int count = 0;
  if (*new_start == INVALID_INDEX) {
    *new_start = *i;
    *new_end = *i;
  }

  for (; line[*i] == ch; (*i)++)
    count++;

  if ((exp == '?' && (count == 0 || count == 1)) ||
  (exp == '+' && count >= 1) ||
  (exp == '*' && count >= 0)) {
    (*j)++;
    (*i)--;
    *new_end += count;
  }
  else {
    *j = 0;
    *new_start = INVALID_INDEX;
    *new_end = INVALID_INDEX;
  }
}


_Bool is_expression(char ch) {
  if (ch == '?' || ch == '+' || ch == '*')
    return TRUE;
  else
    return FALSE;
}


void print_result(char **result, int res_count, const char *pattern, _Bool is_colored, _Bool is_regular) {
  /* Prints result */

  for (int i = 0; i < res_count; i++) {
    if (is_colored)
      print_colored(result[i], pattern, is_regular);
    else
      printf("%s", result[i]);
  }
}


void print_colored(char *line, const char *pattern, _Bool is_regular) {
  /*Prints colored line */

  int start = 0;
  int end = 0;
  int prev_end = 0;

  while (find_pattern(line, pattern, &start, &end, is_regular)) {

    // Before found text
    for (int i = prev_end; i < start; i++)
      printf("%c", line[i]);

    // Colored text
    printf("%c[01;31m%c[K", ESC, ESC);

    for (int i = start; i < end; i++)
      printf("%c", line[i]);

    printf("%c[m%c[K", ESC, ESC);  

    start = end;
    prev_end = end;
  }

  // Rest of the text
  int line_len = my_strlen(line);
  for (int i = prev_end; i < line_len; i++)
    printf("%c", line[i]);
}


void clean_lines(char **lines, int count) {
  /* Cleans array of strings */

  if (!lines)
    return;

  for (int i = 0; i < count; i++)
    free(lines[i]);

  free(lines);
}
