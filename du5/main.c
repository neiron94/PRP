#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_LENGTH 100  //start length for allocation in read_line()
#define TRUE 1
#define FALSE 0
#define MAX_SHIFT 52  //maximal shift in coding algorithm
#define MIN_UPPERCASE 65  //'A'
#define MAX_UPPERCASE 90  //'Z'
#define MIN_LOWERCASE 97  //'a'
#define MAX_LOWERCASE 122 //'z'
#define DIF_1 6
#define DIF_2 58

enum {
  ERROR_OK = EXIT_SUCCESS,
  ERROR_INPUT = 100,
  ERROR_LENGTH = 101,
  ERROR_MEMORY = 102
};


char* read_line(int*);
_Bool is_letter(char);
void clean_line(char**);
char* decode_message(char*, char*, int*);
void shift(char*, char*, int, size_t);
size_t compare(char*, char*, size_t);


int main(int argc, char *argv[])
{
  int error = ERROR_OK;  //variable for monitoring errors
  
  char *encoded_str = read_line(&error);
  // Checking of input errors
  if (error == ERROR_INPUT) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return error;
  }
  else if (error == ERROR_MEMORY) {
    fprintf(stderr, "Error: Neprobehla alokace/realokace!\n");
    return error;
  }

  char *template_str = read_line(&error);
  // Checking of input errors  
  if (error == ERROR_INPUT) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    // Encoded_str has already been written, so we have to clear it
    clean_line(&encoded_str);
    return error;
  }
  else if (error == ERROR_MEMORY) {
    fprintf(stderr, "Error: Neprobehla alokace/realokace!\n");
    clean_line(&encoded_str);
    return error;
  }

  // Checking of length error
  if (strlen(encoded_str) != strlen(template_str)) {
    fprintf(stderr, "Error: Chybna delka vstupu!\n");
    clean_line(&encoded_str);
    clean_line(&template_str);
    return ERROR_LENGTH;
  }

  char* decoded_str = decode_message(encoded_str, template_str, &error);
  if (error == ERROR_MEMORY) {
    fprintf(stderr, "Error: Neprobehla alokace/realokace!\n");
    clean_line(&encoded_str);
    clean_line(&template_str);
    return error;
  }

  printf("%s\n", decoded_str);



  clean_line(&encoded_str);
  clean_line(&template_str);
  clean_line(&decoded_str);

  return ERROR_OK;
}


char* decode_message(char *encoded, char *template, int *error) {
  size_t length = strlen(encoded);
  size_t max_comparison = 0;
  size_t comparisons[MAX_SHIFT];
  for (int i = 0; i < MAX_SHIFT; i++) {
    char *shifted = malloc(sizeof(char) * length + 1);
    if (!shifted) {
      *error = ERROR_MEMORY;
      return NULL;
    }
    shift(shifted, encoded, i, length);
    comparisons[i] = compare(shifted, template, length);
    clean_line(&shifted);
    if (comparisons[i] > max_comparison)
      max_comparison = comparisons[i];
  }

  int key;
  for (int i = 0; i < MAX_SHIFT; i++)
    if (comparisons[i] == max_comparison)
      key = i;

  char* decoded = malloc(sizeof(char) * length + 1);
  if (!decoded) {
      *error = ERROR_MEMORY;
      return NULL;
  }
  shift(decoded, encoded, key, length);

  return decoded;
}

void shift(char* shifted, char* string, int distance, size_t length) {
  for (size_t i = 0; i < length; i++) {
    int result = (int)string[i] + distance;
    if (result > MAX_UPPERCASE && result < MIN_LOWERCASE)
      result += DIF_1;
    else if (result > MAX_LOWERCASE)
      result -= DIF_2;
    shifted[i] = (char)result;
  }
  shifted[length] = '\0';
}

size_t compare(char* shifted, char* template, size_t length) {
  size_t difference = 0;
  for (size_t i = 0; i < length; i++)
    if (shifted[i] == template[i])
      difference++;
  return difference;
}



char* read_line(int *error) {
  /* Function returns new string */

  size_t length = START_LENGTH; //length of the string
  char *string = malloc(sizeof(char) * length); //string to return
  // Checking of a successful allocation
  if (!string) {
    *error = ERROR_MEMORY;
    return NULL;
  }

  // Current_char will be written on current_index in the string
  char current_char;
  size_t current_index = 0;

  while (TRUE) {
    current_char = getchar();

    // If we've run out of allocated memory, then reallocate
    if (current_index == length - 1) {
      length *= 2;
      char *tmp = realloc(string, sizeof(char) * length);
      // Checking of a successful reallocation
      if (!tmp) {
        *error = ERROR_MEMORY;
        clean_line(&string);
        return NULL;
      }
      string = tmp;
    }

    // Checking, if the last symbol ('\n') was written
    if (current_char == '\n') {
      string[current_index] = '\0';
      break;
    }

    // Checking, if current_char is a letter
    if (!is_letter(current_char)){
      *error = ERROR_INPUT;
      clean_line(&string);
      return NULL;
    }
    
    string[current_index] = current_char;
    current_index++;
  }

  // Final reduction of the string
  char *tmp = realloc(string, current_index + 1);
  // Checking of a successful reallocation
  if (!tmp) {
    *error = ERROR_MEMORY;
    clean_line(&string);
    return NULL;
  }
  string = tmp;

  return string;
}


_Bool is_letter(char c) {
  return  (c >= MIN_UPPERCASE && c <= MAX_UPPERCASE) ||
          (c >= MIN_LOWERCASE && c <= MAX_LOWERCASE);
}


void clean_line(char** string) {
  free(*string);
  *string = NULL;
}

