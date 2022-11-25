#include <stdio.h>
#include <stdlib.h>

// void searching(char***, int*, FILE*);

int main () {

    FILE *file = fopen("data/man/pub01-m.in", "r");
    char** lines = NULL;
    lines = (char**)malloc(100 * sizeof(char*));
    int lines_count = 0;

    // searching(&lines, &lines_count, file)


    for (int i = 0; i < 2; i++) {
        char *tmp = malloc(100);
        fgets(tmp, 100, file);
        lines[i] = tmp;
    }


    printf("%s\n", lines[0]);
    printf("%s\n", lines[1]);
    

    fclose(file);
    free(lines);

    return 0;
}


// int searching(char ***result, int *res_count, const char *pattern, FILE *file) {
//   /* Function finds lines with pattern */

//   int ret = EXIT_SUCCESS;

//   *result = (char**)malloc(MAX_SIZE * sizeof(char*));
//   if (!(*result))
//     ret = ERROR_MEMORY;

//   // Find paterns
//   if (ret == EXIT_SUCCESS) {
//     char current_line[MAX_SIZE];
//     while (fgets(current_line, MAX_SIZE, file)) {
//       if (have_pattern()) {
//         *result[*res_count] = current_line;
//         (*res_count)++;
//       }
//     }
//   }

//   return ret;
// }
