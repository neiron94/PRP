/* Used materials */

// 1) Wikipedia Levenshtein distance:
// https://en.wikipedia.org/wiki/Levenshtein_distance

// 2) Wikipedia Wagner–Fischer algorithm:
// https://en.wikipedia.org/wiki/Wagner%E2%80%93Fischer_algorithm


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define START_LENGTH 100  //start length for allocation in read_line()
#define MAX_SHIFT 52  //maximal shift in coding algorithm
#define MIN_UPPERCASE 65  //'A'
#define MAX_UPPERCASE 90  //'Z'
#define MIN_LOWERCASE 97  //'a'
#define MAX_LOWERCASE 122 //'z'
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define TRUE 1


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
void common_decoding(char*, char*, char*, int*);
void shift_line(char*, char*, int);
char shift_letter(char, int);
size_t compare(char*, char*);
void pro_decoding(char*, char*, char*, int*);
int** create_matrix(size_t, size_t, int*);
int find_distance(int**, char*, char*);
void clean_matrix(int***, size_t);


int main(int argc, char *argv[]) {

  int error = ERROR_OK;  //variable for monitoring errors
  char *encoded_line = NULL; 
  char *template_line = NULL;
  char *decoded_line = NULL;

  encoded_line = read_line(&error);

  if (error == ERROR_OK)
    template_line = read_line(&error);
  
  // Checking of length error 
  /*if argument "-prp-optional" wasn't written and
  error isn't already ERROR_INPUT and lines have different lengths*/
  if (!(argc == 2 && !strcmp(argv[1], "-prp-optional")) && 
  error != ERROR_INPUT && strlen(encoded_line) != strlen(template_line))
      error = ERROR_LENGTH;

  if (error == ERROR_OK)
    decoded_line = decode_message(encoded_line, template_line, &error);

  if (error == ERROR_OK)
    printf("%s\n", decoded_line);

  // Output, if errors were detected
  if (error == ERROR_INPUT)
    fprintf(stderr, "Error: Chybny vstup!\n");
  else if (error == ERROR_LENGTH)
    fprintf(stderr, "Error: Chybna delka vstupu!\n");
  else if (error == ERROR_MEMORY)
    fprintf(stderr, "Error: Neprobehla alokace/realokace!\n");
  
  clean_line(&encoded_line);
  clean_line(&template_line);
  clean_line(&decoded_line);

  return error;
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
  char *tmp = realloc(string, sizeof(char) * (current_index + 1));
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
  if (*string != NULL) {
    free(*string);
    *string = NULL;
  }
}


char* decode_message(char *encoded, char *template, int *error) {
  /* Function returns decoded line */

  // Decoded string has the same length as encoded
  char *decoded = malloc(sizeof(char) * (strlen(encoded) + 1));
  // Checking of a successful allocation
  if (!decoded)
      *error = ERROR_MEMORY;

  if (*error == ERROR_OK) {
    _Bool are_same = strlen(encoded) == strlen(template);
    if (are_same)
      common_decoding(decoded, encoded, template, error);
    else
      pro_decoding(decoded, encoded, template, error);
  }

  return decoded;
}


void common_decoding(char *decoded, char *encoded, char *template, int *error) {
  /* Function goes throw all possible decoded strings and compares
  them to template, then returns one with the biggest number of matches */

  size_t length = strlen(encoded);
  size_t max_comparison = 0;  //the biggest number of matches
  size_t comparisons[MAX_SHIFT];  //store numbers of matches

  // Go throw all decoding keys
  for (int key = 0; key < MAX_SHIFT; key++) {

    // Current decoded (shifted) string
    char *shifted = malloc(sizeof(char) * (length + 1));
    // Checking of a successful allocation
    if (!shifted) {
      *error = ERROR_MEMORY;
      return;
    }

    // Decode (shift) current line with current key
    shift_line(shifted, encoded, key);

    // Store new result of comparison and compare it to current max_comparison
    comparisons[key] = compare(shifted, template);
    if (comparisons[key] > max_comparison)
      max_comparison = comparisons[key];

    clean_line(&shifted);
  }

  // Find true decoding key and decode string with it
  int real_key;
  for (int i = 0; i < MAX_SHIFT; i++)
    if (comparisons[i] == max_comparison)
      real_key = i;

  shift_line(decoded, encoded, real_key);
}


void shift_line(char *shifted, char *string, int key) {
  /* Function shifts 'string' by the 'key' and stores result in 'shifted' */

  size_t length = strlen(string);

  for (size_t i = 0; i < length; i++)
    shifted[i] = shift_letter(string[i], key);

  shifted[length] = '\0';
}


char shift_letter(char letter, int key) {
  /* Function shifts one 'letter' by the 'key' and returns 'result' */
  char result = '\0';

  // Go throw 'ALPHABET' and find the 'letter'
  for (int i = 0; i < MAX_SHIFT; i++) {
    if (letter == ALPHABET[i]) {
      // Shift the 'letter'
      int index = (i + key) % MAX_SHIFT;
      result = ALPHABET[index];
      break;
    }
  }

  return result;
}


size_t compare(char* first, char* second) {
  /* Function compares two strings and returns the number of matches */
  
  size_t matches = 0;
  size_t length = strlen(first);

  for (size_t i = 0; i < length; i++)
    if (first[i] == second[i])
      matches++;
  
  return matches;
}


void pro_decoding(char *decoded, char *encoded, char *template, int *error) {
  /* Function goes throw all possible decoded strings and compares
  them to template using Levenshtein distance and Wagner–Fischer algorithm,
  then returns one with the smallest distance*/

  size_t encoded_len = strlen(encoded);
  size_t template_len = strlen(template);
  int distances[MAX_SHIFT];
  int min_distance = INT_MAX;

  // Matrix is used for Wagner–Fischer algorithm
  int **matrix = create_matrix(template_len + 1, encoded_len + 1, error);
  // Checking of a successful allocation
  if (*error == ERROR_MEMORY)
    return;
  
  // Go throw all decoding keys
  for (int key = 0; key < MAX_SHIFT; key++) {

    // Current decoded (shifted) string
    char *shifted = malloc(sizeof(char) * (encoded_len + 1));
    // Checking of a successful allocation
    if (!shifted) {
      *error = ERROR_MEMORY;
      clean_matrix(&matrix, template_len + 1);
      return;
    }

    // Decode (shift) current line with current key
    shift_line(shifted, encoded, key);

    // Find current distance and compare it to current min_distance
    distances[key] = find_distance(matrix, template, shifted);
    if (distances[key] < min_distance)
      min_distance = distances[key];

    clean_line(&shifted);
  }

  clean_matrix(&matrix, template_len + 1);

  // Find true decoding key and decode string with it
  int real_key;
  for (int i = 0; i < MAX_SHIFT; i++)
    if (distances[i] == min_distance)
      real_key = i;

  shift_line(decoded, encoded, real_key);
}


int** create_matrix(size_t n, size_t m, int *error) {
  /* Function creates matrix n x m */

  int **matrix = (int**)malloc(sizeof(int*) * n);
  // Checking of a successful allocation
  if (!matrix) {
    *error = ERROR_MEMORY;
    return NULL;
  }

  for (size_t i = 0; i < n; i++) {
    matrix[i] = (int*)malloc(sizeof(int) * m);
    // Checking of a successful allocation
    if (!matrix[i]) {
      clean_matrix(&matrix, i);
      *error = ERROR_MEMORY;
      return NULL;
    }
  }
  
  // Fill initial values for first column
  for (size_t i = 0; i < n; i++)
    matrix[i][0] = i;
  // Fill initial values for first row
  for (size_t j = 0; j < m; j++)
    matrix[0][j] = j;

  return matrix;
}


int find_distance(int **matrix, char *first, char *second) {
  /* Function finds Levenshtein distance between two words */

  // Sizes of matrix n x m
  size_t n = strlen(first);
  size_t m = strlen(second);

  for (size_t i = 1; i <= n; i++) {
    for (size_t j = 1; j <= m; j++) {

      int substitutionCost;
      if (first[i - 1] == second[j - 1])
        substitutionCost = 0;
      else
        substitutionCost = 1;

      int deletion = matrix[i - 1][j] + 1;
      int insertion = matrix[i][j - 1] + 1;
      int substitution = matrix[i - 1][j - 1] + substitutionCost;

      int minimum = deletion < insertion ? deletion : insertion;
      minimum = minimum < substitution ? minimum : substitution;

      matrix[i][j] = minimum;
    }
  }

  return matrix[n][m];
}


void clean_matrix(int ***matrix, size_t n) {

  for (size_t i = 0; i < n; i++)
    free((*matrix)[i]);

  free(*matrix);
  *matrix = NULL;
}
