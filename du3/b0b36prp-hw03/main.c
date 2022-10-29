#include <stdio.h>
#include <stdlib.h>

//bounderies for width and height
#define LOWER_BOUNDERY 3
#define UPPER_BOUNDERY 69


void draw_roof(int);
void draw_common_bottom(int, int);
void draw_pro_bottom(int, int, int);
void draw_fence(int, int, int);

int main(int argc, char *argv[])
{
  int width, height, fence_size;
  //input check
  if (scanf("%d %d", &width, &height) != 2) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
  if (width == height) {
    if(scanf("%d", &fence_size) != 1) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      return 100;
    }
  }
  if (width < LOWER_BOUNDERY || width > UPPER_BOUNDERY ||
      height < LOWER_BOUNDERY || height > UPPER_BOUNDERY) {
    fprintf(stderr, "Error: Vstup mimo interval!\n");
    return 101;
  }
  if (width % 2 != 1) {
    fprintf(stderr, "Error: Sirka neni liche cislo!\n");
    return 102;
  }
  if (width == height) {
    if (fence_size <= 0 || fence_size >= height) {
      fprintf(stderr, "Error: Neplatna velikost plotu!\n");
      return 103;
    }
  }

  draw_roof(width);

  if (width != height)
    draw_common_bottom(width, height);
  else 
    draw_pro_bottom(width, height, fence_size);
  
  return 0;
}


void draw_roof(int width) {
  //roof always has height = width / 2
  int roof_height = width / 2;

  for (int i = 0; i < roof_height; i++) {
    int X_count = 0;
    for (int j = 0; j < width; j++) {
      char current_char = ' ';

      if (j == roof_height + i || j == roof_height - i) {
        current_char = 'X';
        X_count++;
      }
      printf("%c", current_char);
      //if its the last 'X' in a row
      if ((i == 0 && X_count == 1) || X_count == 2)
        break;
    }
    printf("\n");
  }
}


void draw_common_bottom(int width, int height) {

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      char current_char = ' ';
      
      if (j == 0 || j == width - 1 || //left and right borders of house
          i == 0 || i == height - 1)  //upper and lower borders of house
        current_char = 'X';

      printf("%c", current_char);
    }

    printf("\n");
  }
}


void draw_pro_bottom(int width, int height, int fence_size) {

  for (int i = 0; i < height; i++) {
    //draws bottom part of house without a fence
    for (int j = 0; j < width; j++) {
      char current_char;
      
      if (j == 0 || j == width - 1 || //left and right borders of house
          i == 0 || i == height - 1)  //upper and lower borders of house
        current_char = 'X';
      else if ((i + j) % 2 == 0)      //inside of house (o)
        current_char = 'o';
      else                            //inside of house (*)
        current_char = '*';
      printf("%c", current_char);
    }

    draw_fence(fence_size, height, i);
    printf("\n");
  }
}


void draw_fence(int fence_size, int height, int current_iteration) {
  //index of start of fence
  int fence_start = height - fence_size;

  //if fence has already started
  if (current_iteration >= fence_start) {
    for (int i = 0; i < fence_size; i++) {
      char current_char;

      //if its an upper or lower part of a fence
      if (current_iteration == fence_start || current_iteration == height - 1)
        current_char = '-';
      //if its in a middle of fence
      else
        current_char = ' ';
      //conditionals for drawing '|'
      if ((fence_size % 2 == 0 && i % 2 == 1) ||
          (fence_size % 2 == 1 && i % 2 == 0))
        current_char = '|';

      printf("%c", current_char);
    }
  }
}
