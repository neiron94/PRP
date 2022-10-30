#include <stdio.h>
#include <math.h>

//we will use prime numbers, which are <10^6
#define MAX_PRIME_NUMBER (int)pow(10, 6)
#define TRUE 1
#define FALSE 0

//possible return values
enum {
  EXIT_SUCCESS = 0,
  INPUT_ERROR = 100
};

void sieve_of_Eratosthenes(long*);
void prime_decomposition(long, long*);
long next_prime(long, long*);
void print_result(long, long, int*);


int main(int argc, char *argv[])
{
  //creats array of prime numbers 
  long prime_numbers[MAX_PRIME_NUMBER];
  //fills array of prime numbers
  sieve_of_Eratosthenes(prime_numbers);

  int input;
  long current_number;

  while (TRUE) {
    //reads current number and scanf return value
    input = scanf("%lu", &current_number);
    
    //loop exit
    if (current_number == 0)
      break;

    //if current number is negative or other wrong input
    if (current_number < 0 || input != 1) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      return INPUT_ERROR;
    }

    //prime decomposition of current number (with displaying the result)
    prime_decomposition(current_number, prime_numbers);
  }

  return EXIT_SUCCESS;
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


void prime_decomposition(long number, long* prime_numbers) {
  printf("Prvociselny rozklad cisla %lu je:\n", number);

  //specific situation
  if (number == 1) {
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
  while (number != 1) {
    if (number % current_divisor == 0) {
      divisor_count++;
      number /= current_divisor;
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
