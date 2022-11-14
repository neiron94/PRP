#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_LENGTH 100
#define TRUE 1
#define FALSE 0

enum {
  ERROR_OK = EXIT_SUCCESS,
  ERROR_INPUT = 100,
  ERROR_LENGTH = 101,
  ERROR_MEMORY = 102
};


int read_line(char*, size_t*);
_Bool is_letter(char);
void clean_line(char**);
char* reduce(char*, size_t);


int main(int argc, char *argv[])
{
  size_t encoded_len = START_LENGTH;
  char *encoded_str = malloc(sizeof(char) * encoded_len);
  if (!encoded_str)
    return ERROR_MEMORY;
  int function_ret = read_line(encoded_str, &encoded_len);
  if (function_ret == ERROR_INPUT) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return ERROR_INPUT;
  }
  else if (function_ret == ERROR_MEMORY) {
    fprintf(stderr, "Error: Neprobehla alokace/realokace!\n");
    return ERROR_MEMORY;
  }

  size_t eavesdropped_len = START_LENGTH;
  char *eavesdropped_str = malloc(sizeof(char) * eavesdropped_len);
  if (!eavesdropped_str)
    return ERROR_MEMORY;
  function_ret = read_line(eavesdropped_str, &eavesdropped_len);
  if (function_ret == ERROR_INPUT) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return ERROR_INPUT;
  }
  else if (function_ret == ERROR_MEMORY) {
    fprintf(stderr, "Error: Neprobehla alokace/realokace!\n");
    return ERROR_MEMORY;
  }

  if (encoded_len != eavesdropped_len) {
    fprintf(stderr, "Error: Chybna delka vstupu!\n");
    return ERROR_LENGTH;
  }

  //char* decoded_str;



  clean_line(&encoded_str);
  clean_line(&eavesdropped_str);
  //clean_line(&decoded_str);

  return ERROR_OK;
}


int read_line(char *string, size_t *length) {

  char current_char;
  size_t current_index = 0;
  while ((current_char = getchar()) != '\n') {
    if (!is_letter(current_char))
      return ERROR_INPUT;
    
    if (current_index >= *length) {
      *length *= 2;
      char *tmp = realloc(string, sizeof(char) * (*length));
      if (!tmp) {
        clean_line(&string);
        return ERROR_MEMORY;
      }
      string = tmp;
    }

    string[current_index] = current_char;
    current_index++;
  }

  *length = current_index;

  string = reduce(string, (*length) + 1);
  
  char *tmp = realloc(string, (*length) + 1);
  if (!tmp) {
    clean_line(&string);
    return ERROR_MEMORY;
  }
  string = tmp;
  string[current_index] = '\0';

  return ERROR_OK;
}

char* reduce(char* string, size_t len) {
  char *tmp = realloc(string, len);
  
  return tmp;
}
_Bool is_letter(char c) {
  return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) ? TRUE : FALSE;
}


void clean_line(char** string) {
  free(*string);
  *string = NULL;
}

