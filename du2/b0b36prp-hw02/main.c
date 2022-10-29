#include <stdio.h>
#include <stdlib.h>

// -10001 is smaller then every number in [-10000; 10000]
#define FIRST_MAX -10001
// 10001 is bigger then every number in [-10000; 10000]
#define FIRST_MIN 10001
//boundaries of interval [-10000; 10000]
#define LOWER_BOUNDARY -10000
#define UPPER_BOUNDERY 10000

void write_number(int, int);
void count_properties(int, int*, int*, int*, int*, int*, int*, int*, int*);
void write_information(int, int, int, int, int, int, int, int, int);


int main(int argc, char *argv[])
{
  int current_number, number_count = 0,
      max_number = FIRST_MAX, min_number = FIRST_MIN,
      odd_count = 0, even_count = 0,
      positive_count = 0, negative_count = 0,
      numbers_sum = 0;

  //input until end or error
  while (1) {
    int input = scanf("%d", &current_number);

    if (input == 1) {
      if (current_number >= LOWER_BOUNDARY && current_number <= UPPER_BOUNDERY) {
        write_number(current_number, number_count);
        count_properties(current_number, &number_count,
                         &max_number, &min_number,
                         &odd_count, &even_count,
                         &positive_count, &negative_count,
                         &numbers_sum);
      }
      else {
        printf("\nError: Vstup je mimo interval!\n");
        return 100;
      }
    }
    else  break; //breaks if error (input == 0) or EOF
  }

  write_information(current_number, number_count,
                    max_number, min_number,
                    odd_count, even_count,
                    positive_count, negative_count,
                    numbers_sum);
 
  return 0;
}


void write_number(int current_number, int number_count) {
  //write ", " before all numbers, but the first 
  if (number_count == 0)
    printf("%d", current_number);
  else
    printf(", %d", current_number);
}


void count_properties(int current_number, int* number_count,
                      int* max_number, int* min_number,
                      int* odd_count, int* even_count,
                      int* positive_count, int* negative_count,
                      int* numbers_sum) {

  (*number_count)++;
  *numbers_sum += current_number;
  
  if (current_number > *max_number) *max_number = current_number;
  if (current_number < *min_number) *min_number = current_number;
  
  if (current_number % 2 == 0)      (*even_count)++;
  else                              (*odd_count)++;

  if (current_number > 0)           (*positive_count)++;
  if (current_number < 0)           (*negative_count)++;
}


void write_information(int current_number, int number_count,
                       int max_number, int min_number,
                       int odd_count, int even_count,
                       int positive_count, int negative_count,
                       int numbers_sum) {
  
  printf("\nPocet cisel: %d\n", number_count);
  printf("Pocet kladnych: %d\n", positive_count);
  printf("Pocet zapornych: %d\n", negative_count);
  printf("Procento kladnych: %.2f\n", (float)positive_count / number_count * 100);
  printf("Procento zapornych: %.2f\n", (float)negative_count / number_count * 100);
  printf("Pocet sudych: %d\n", even_count);
  printf("Pocet lichych: %d\n", odd_count);
  printf("Procento sudych: %.2f\n", (float)even_count / number_count * 100);
  printf("Procento lichych: %.2f\n", (float)odd_count / number_count * 100);
  printf("Prumer: %.2f\n", (float)numbers_sum / number_count);
  printf("Maximum: %d\n", max_number);
  printf("Minimum: %d\n", min_number);
}
