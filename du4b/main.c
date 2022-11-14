#include <stdio.h>
#include <math.h>

//we will use prime numbers, which are <10^6
#define MAX_PRIME_NUMBER (int)pow(10, 6)
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
void big_number_div(int*, int, long);



int main(int argc, char *argv[])
{
  //creats array of prime numbers 
  long prime_numbers[MAX_PRIME_NUMBER];
  //fills array of prime numbers
  sieve_of_Eratosthenes(prime_numbers);

  while(TRUE) {
    int number[100];
    int count = 0;
    char current_number;

    //puts number in array
    while ((current_number = getchar()) != '\n') {

      //if current number is negative or other wrong input
      if (current_number < ASCII_0 || current_number > ASCII_9) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return INPUT_ERROR;
      }

    number[count] = current_number - ASCII_0;
    count++;

    if (number[0] == 0)
      return EXIT_SUCCESS;
    }

    prime_decomposition(number, count, prime_numbers);
  }
}


void sieve_of_Eratosthenes(long* prime_numbers) {
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


void prime_decomposition(int* number, int count, long* prime_numbers) {
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
  //at first we will try to divide by 2
  long current_divisor = 2;
  //variable for proper printing of " x "
  int is_first_to_print = TRUE;

  //we will divide number and print result, until it's equals 1
  while (!is_equals_one(number, count)) {
    if (mod_equals_zero(number, count, current_divisor)) {
      divisor_count++;
      big_number_div(number, count, current_divisor);
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


long next_prime(long current_divisor, long* prime_numbers) {
  //increments current_divisor, until it's a prime number
  do {
    current_divisor++;
  } while (prime_numbers[current_divisor] != 0);

  return current_divisor;
}


void print_result(long divisor, long divisor_count, int* is_first_to_print) {
  //if it isn't first result of division
  if (!(*is_first_to_print)) 
    printf(" x ");

  printf("%lu", divisor);

  if (divisor_count > 1)
    printf("^%lu", divisor_count);

  *is_first_to_print = FALSE;
}


int is_equals_one(int* number, int count) {
  for (int i = 0; i < count - 1; i++)
    if (number[i] != 0)
      return FALSE;

  if (number[count - 1] != 1)
      return FALSE;

  return TRUE;
}


int mod_equals_zero(int* origin, int count, long divisor) {
  int number[100];
  for (int i = 0; i < count; i++)
    number[i] = origin[i];

  while (TRUE) {
    int current_last_index = 0;
    int count_zeroes = 0;

    //finds first not zero number
    while (number[current_last_index] == 0) {
      count_zeroes++;
      current_last_index++;
      //true function exit
      if (current_last_index == count)
        return TRUE;
    }

    long current_dividend = number[current_last_index];

    while (current_dividend < divisor) {
      current_last_index++;
      //false function exit
      if (current_last_index == count)
        return FALSE;
      
      current_dividend = 0;
      for (int i = count_zeroes, k = 0; i <= current_last_index; i++, k++)
        current_dividend += number[i] * pow(10, current_last_index - count_zeroes - k);
    }

    long partial_mod = current_dividend % divisor;

    for (int i = current_last_index; i >= 0; i--) {
      number[i] = partial_mod % 10;
      partial_mod /= 10;
    }
  }
}


void big_number_div(int* origin, int count, long divisor) {
  int new_origin[100];
  int new_count = 0;
  int number[100];
  for (int i = 0; i < count; i++)
    number[i] = origin[i];

  while (TRUE) {
    int current_last_index = 0;
    int count_zeroes = 0;

    //finds first not zero number
    while (number[current_last_index] == 0) {
      count_zeroes++;
      current_last_index++;
      //true function exit
      if (current_last_index == count)
        goto loop_exit;
    }

    long current_dividend = number[current_last_index];

    while (current_dividend < divisor) {
      current_last_index++;
      //false function exit
      if (current_last_index == count)
        goto loop_exit;
      
      current_dividend = 0;
      for (int i = count_zeroes, k = 0; i <= current_last_index; i++, k++)
        current_dividend += number[i] * pow(10, current_last_index - count_zeroes - k);
    }

    long partial_mod = current_dividend % divisor;

    for (int i = current_last_index; i >= 0; i--) {
      number[i] = partial_mod % 10;
      partial_mod /= 10;
    }

    new_origin[new_count] = current_dividend / divisor;
    new_count++;
  }

  loop_exit:
    for (int i = count - 1, k = 0; i >= 0; i--, k++)
      if (k < new_count)
        number[i] = new_origin[new_count - k - 1];
}
