#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[]) {
    int a, b;
    if (scanf("%d %d", &a, &b) != 2)
        return 1;
    if (a < -10000 || a > 10000 || b < -10000 || b > 10000) {
        fprintf(stderr, "Error: Vstup je mimo interval!\n");
        return 100;
    }

    printf("Desitkova soustava: %d %d\n", a, b);
    printf("Sestnactkova soustava: %x %x\n", a, b);
    int soucet = a + b;
    printf("Soucet: %d + %d = %d\n", a, b, soucet);
    int rozdil = a - b;
    printf("Rozdil: %d - %d = %d\n", a, b, rozdil);
    int soucin = a * b;
    printf("Soucin: %d * %d = %d\n", a, b, soucin);
    float podil;
    if (b == 0) {
        podil = NAN;
        fprintf(stderr, "Error: Nedefinovany vysledek!\n");
        printf("Podil: %d / %d = NaN\n", a, b);
    }
    else {
        podil = (int)(a / b);
        printf("Podil: %d / %d = %.0f\n", a, b, podil);
    }
    float prumer = (a + b) / 2.0;
    printf("Prumer: %.1f\n", prumer);

    if (b == 0) return 101;
    return 0; 
}
