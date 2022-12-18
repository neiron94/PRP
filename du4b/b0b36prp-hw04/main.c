#include <stdio.h>
#include <math.h>


//we will use prime numbers, which are <10^6
#define MAX_PRIME_NUMBER 1000000
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 100
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
int big_number_div(int*, int*, long);
int count_digits(long);
void big_number_minus(int*, int*, int*, int);
void increase_result(int*, int*, int);
int is_equals_zero(int*, int);
int num_is_bigger_than_div(int*, int, int*, int);



int main(int argc, char *argv[])
{
  // int test[100];
  // int count = 5;
  // test[0] = 1;
  // test[1] = 1;
  // test[2] = 0;
  // test[3] = 0;
  // test[4] = 4;

  // printf("Count = %d\nNumber: ", count);
  // printf("%lu\n", number_as_long(test, count));

  // big_number_div(test, &count, 5);

  // printf("Count = %d\nNumber: ", count);
  // printf("%lu\n", number_as_long(test, count));

  // return 0;

  //creats array of prime numbers 
  long prime_numbers[MAX_PRIME_NUMBER];
  //fills array of prime numbers
  sieve_of_Eratosthenes(prime_numbers);

  while(TRUE) {
    int number[MAX_SIZE];
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
    if (mod_equals_zero(number, count, current_divisor)) {
      divisor_count++;
      big_number_div(number, &count, current_divisor);
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

  // Special cases
  int digit_sum = 0;
  for (int i = 0; i < count; i++)
    digit_sum += origin[i];

  if (divisor == 2)
    return origin[count - 1] % 2 == 0;
  if (divisor == 3)
    return digit_sum % 3 == 0;
  if (divisor == 5)
    return origin[count - 1] == 0 || origin[count - 1] == 5;
  
  // Copy of the origin number
  int number[100];
  for (int i = 0; i < count; i++)
    number[i] = origin[i];

  return big_number_div(number, &count, divisor);

}


int big_number_div(int *origin, int *count, long divisor) {
  /* Divide number. Returns number % divisor == 0 */

  // Result of division
  int result[MAX_SIZE];
  for (int i = 0; i < MAX_SIZE; i++)
    result[i] = 0;
  int res_len = 0;

  // Copy the origin number
  int copy_number[MAX_SIZE];
  for (int i = 0; i < *count; i++)
    copy_number[i] = origin[i];
  int copy_count = *count;

  // Represent divisor as an array
  int div_as_arr[MAX_SIZE];
  for (int i = 0; i < MAX_SIZE; i++)
    div_as_arr[i] = 0;
  int div_len = count_digits(divisor);
  for (int div_copy = divisor, i = div_len - 1; i >= 0; i--) {
    div_as_arr[i] = div_copy % 10;
    div_copy /= 10;
  }

  // Algorithm of division
  while (num_is_bigger_than_div(copy_number, copy_count, div_as_arr, div_len)) {
    int cur_div_len = div_len;

    // Charging
    while (cur_div_len < copy_count)
      cur_div_len++;
    // Correction
    if (!num_is_bigger_than_div(copy_number, copy_count, div_as_arr, cur_div_len))
      cur_div_len--;

    // Substractions
    do {
      big_number_minus(copy_number, &copy_count, div_as_arr, cur_div_len);
      int mult = cur_div_len - div_len;
      increase_result(result, &res_len, mult);
    } while (num_is_bigger_than_div(copy_number, copy_count, div_as_arr, cur_div_len));
  }

  // Set origin number on result
  if (is_equals_zero(copy_number, copy_count)) {
    *count = res_len;
    for (int i = 0; i < res_len; i++)
      origin[res_len - 1 - i] = result[MAX_SIZE - 1 - i];

    return TRUE;
  }
  else {
    return FALSE;
  }
}


int is_equals_zero(int *number, int count) {
  /* Desides if number is zero */

  for (int i = 0; i < count; i++) {
    if (number[i] != 0)
      return FALSE;
  }

  return TRUE;
}


int count_digits(long number) {
  /* Counts digits in number */

  int count = 0;
    while (number > 0) {
    number /= 10;
    count++;
  }

  return count;
}


void big_number_minus(int *number, int *num_len, int *divisor, int div_len) {

  // Minus
  for (int i = 0; i < div_len; i++) {
    if (number[(*num_len) - 1 - i] >= divisor[div_len - 1 - i]) {
      number[(*num_len) - 1 - i] -= divisor[div_len - 1 - i];
    }
    else {
      number[(*num_len) - 2 - i]--;
      number[(*num_len) - 1 - i] = 10 + number[(*num_len) - 1 - i] - divisor[div_len - 1 - i];
      
      for (int j = (*num_len) - 2 - i; j >= 0; j--) {
        if (number[j] == -1) {
          number[j] = 9;
          number[j - 1]--;
        }
      }
    }
  }

  // Move digits in number to the right if needed
  int count_zeroes = 0;
  for (int i = 0; number[i] == 0; i++)
    count_zeroes++;

  if (count_zeroes == (*num_len)) {
    *num_len = 1;
  }
  else {
    *num_len = (*num_len) - count_zeroes;
    for (int i = 0; i < *num_len; i++) {
      number[i] = number[i + count_zeroes];
    }
  }
}


void increase_result(int *result, int *res_len, int mult) {
  if (mult + 1 > *res_len)
    *res_len = mult + 1;

  result[MAX_SIZE - 1 - mult]++;

  if (result[MAX_SIZE - 1] == 10) {
    for (int i = 0; i < *res_len; i++) {
      if (result[MAX_SIZE - 1 - i] == 10) {
        result[MAX_SIZE - 1 - i] = 0;
        result[MAX_SIZE - 2 - i]++;
        if (i == (*res_len) - 1)
          (*res_len)++; 
      }
    }
  }
}


int num_is_bigger_than_div(int *number, int num_len, int *divisor, int div_len) {
  if (num_len > div_len)
    return TRUE;
  else if (num_len < div_len)
    return FALSE;
  else {
    for (int i = 0; i < num_len; i++) {
      if (number[i] > divisor[i])
        return TRUE;
      else if (number[i] < divisor[i])
        return FALSE;
    }
  }
  return TRUE;
}
