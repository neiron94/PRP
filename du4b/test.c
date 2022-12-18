#include <stdio.h>
#include <math.h>


//we will use prime numbers, which are <10^6
#define MAX_PRIME_NUMBER 1000000
#define TRUE 1
#define FALSE 0
//indexes of "0" and "9" in ASCII
#define ASCII_0 48
#define ASCII_9 57

//possible return values
enum {
  EXIT_SUCCESS = 0,
  INPUT_ERROR = 100
};

void sieve_of_Eratosthenes(long*);
void prime_decomposition(int*, int, long*);
long next_prime(long, long*);
void print_result(long, long, int*);
int is_equals_one(int*, int);
int mod_equals_zero(int*, int, long);
long big_number_div(int*, int*, long);
int count_digits(long);
long number_as_long(int*, int);
void big_number_minus(int*, int*, long);



int main(int argc, char *argv[])
{
  //creats array of prime numbers 
  long prime_numbers[MAX_PRIME_NUMBER];
  //fills array of prime numbers
  sieve_of_Eratosthenes(prime_numbers);

  while(TRUE) {
    int number[100];
    int dig_count = 0;  // count of digits
    char current_digit;

    //puts number in array
    while ((current_digit = getchar()) != '\n') {

      //if current number is negative or other wrong input
      if (current_digit < ASCII_0 || current_digit > ASCII_9) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return INPUT_ERROR;
      }

    number[dig_count] = current_digit - ASCII_0;
    dig_count++;

    if (number[0] == 0)
      return EXIT_SUCCESS;
    }

    prime_decomposition(number, dig_count, prime_numbers);
  }
}


void sieve_of_Eratosthenes(long *prime_numbers) {
  /* "0" on index i in prime_numbers array means i is a prime number,
     "1" means i isn't a prime number */

  //filling array with "0"
  for (int i = 0; i < MAX_PRIME_NUMBER; i++)
    prime_numbers[i] = 0;
  
  //we will just ignore numbers "1" and "0"
  prime_numbers[0] = 1;
  prime_numbers[1] = 1;

  //classical algorithm of "Sieve of Eratosthenes"
  for (int i = 2; i < (int)sqrt(MAX_PRIME_NUMBER); i++) {

    //this condition will save us time, we will check
    //other number only if current number is prime
    if (prime_numbers[i] == 0)
      //all numbers from i^2 with step i aren't prime
      for (int k = i * i; k < MAX_PRIME_NUMBER; k += i)
        prime_numbers[k] = 1;
  }

}


void prime_decomposition(int *number, int count, long *prime_numbers) {
  //first string of output
  printf("Prvociselny rozklad cisla ");
  for (int i = 0; i < count; i++)
    printf("%d", number[i]);
  printf(" je:\n");

  //specific situation
  if (number[0] == 1 && count == 1) {
    printf("1\n");
    return;
  }

  //divisor_count represents count of the same divisors
  //8=2*2*2, divisor_count for "2" = 3
  long divisor_count = 0;
  //firstly, we will try to divide by 2
  long current_divisor = 2;
  //variable for proper printing of " x "
  int is_first_to_print = TRUE;

  //we will divide number and print result, until it's equals 1
  while (!is_equals_one(number, count)) {

    printf("Checkpoint 1\n"); // DEBUG
    
    if (mod_equals_zero(number, count, current_divisor)) {

      printf("Checkpoint 2\n"); // DEBUG

      divisor_count++;
      big_number_div(number, &count, current_divisor);

      printf("Checkpoint 3\n"); // DEBUG

    }
    //if we can't divide number by current_divisor
    else {
      //if we have divided number by current_divisor at least once
      if (divisor_count > 0)
        print_result(current_divisor, divisor_count, &is_first_to_print);
      
      divisor_count = 0;
      //current_divisor changes to next prime number
      current_divisor = next_prime(current_divisor, prime_numbers);
    }
  }
  //we should print results of division one more time, because it
  //wasn't printed on the last iteration of the while-loop
  print_result(current_divisor, divisor_count, &is_first_to_print);
  
  printf("\n");
}


long next_prime(long current_divisor, long *prime_numbers) {
  //increments current_divisor, until it's a prime number
  do {
    current_divisor++;
  } while (prime_numbers[current_divisor] != 0);

  return current_divisor;
}


void print_result(long divisor, long divisor_count, int *is_first_to_print) {
  //if it isn't first result of division
  if (!(*is_first_to_print)) 
    printf(" x ");

  printf("%lu", divisor);

  if (divisor_count > 1)
    printf("^%lu", divisor_count);

  *is_first_to_print = FALSE;
}


int is_equals_one(int *number, int count) {
  return number[0] == 1 && count == 1 ? TRUE : FALSE;
}


int mod_equals_zero(int *origin, int count, long divisor) {

  // Copy of the origin number
  int number[100];
  for (int i = 0; i < count; i++)
    number[i] = origin[i];

  return big_number_div(number, &count, divisor) == 0 ? TRUE : FALSE;

}


long big_number_div(int *origin, int *count, long divisor) {
  /* Divide number and returns the remainder of the division */

  // Copy of the origin number
  int number[100];
  for (int i = 0; i < *count; i++)
    number[i] = origin[i];

  int div_len = count_digits(divisor);

  while (*count > div_len)
    big_number_minus(number, count, divisor);

  while (number_as_long(number, *count) >= divisor)
    big_number_minus(number, count, divisor);

  return number_as_long(number, *count);
}


int count_digits(long number) {
  int count = 0;
    while (number > 0) {
    number /= 10;
    count++;
  }

  return count;
}


long number_as_long(int *number, int count) {
  long ret = 0;
  for (int i = 0; i < count; i++)
    ret += number[count - 1 - i] * pow(10, i);

  return ret;
}


void big_number_minus(int *number, int *count, long divisor) {
  
  int div_len = count_digits(divisor);

  // Represent divisor as an array
  int div_as_arr[div_len];
  long div_copy = divisor;
  for (int i = div_len - 1; i >= 0; i--) {
    div_as_arr[i] = div_copy % 10;
    div_copy /= 10;
  }

  // Minus
  for (int i = 0; i < div_len; i++) {
    if (number[(*count) - 1 - i] >= div_as_arr[div_len - 1 - i]) {
      number[(*count) - 1 - i] -= div_as_arr[div_len - 1 - i];
    }
    else {
      number[(*count) - 2 - i] -= 1;
      number[(*count) - 1 - i] = (number[(*count) - 1 - i] * 10) - div_as_arr[div_len - 1 - i];
    }
  }

  // Move digits in number to the right if needed
  int count_zeroes = 0;
  for (int i = 0; number[i] == 0; i++)
    count_zeroes++;

  if (count_zeroes == (*count)) {
    *count = 1;
  }
  else {
    *count -= count_zeroes;
    for (int i = 0; i < *count; i++) {
      number[i] = number[i + count_zeroes];
    }
  }
}
