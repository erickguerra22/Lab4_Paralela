#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

bool isNumber(const char *str) {
    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

// Función a integrar: x^2
double f(double x) {
    return pow(x, 2);
}

double riemann(double a, double b, int n) {
    double sum = 0.0;
    double dx = (b - a) / n;

    for (int i = 0; i < n; i++) {
        double x = a + i * dx;
        sum += f(x) * dx;
    }

    return sum;
}

int main(int argc, char *argv[]) {
    double a = 5.0, b = 25.0;
    int n = 10000;
    
    if (argc > 1) {
        if (isNumber(argv[1])) {
            a = strtod(argv[1], NULL);
        } else {
            printf("Error: el valor para 'a' no es válido.\n");
            return 1;
        }
        if (argc > 2 && isNumber(argv[2])) {
            b = strtod(argv[2], NULL);
        } else {
            printf("Error: el valor para 'b' no es válido.\n");
            return 1;
        }
        if (argc > 3 && isNumber(argv[3])) {
            n = atoi(argv[3]);
        } else if (argc > 3) {
            printf("Error: el valor para 'n' no es válido. Utilizando valor por defecto.\n");
            n = 10000;
        }
    }

    clock_t start_time = clock();
    
    double result = riemann(a, b, n);
    
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Resultado aproximado de la integral x^2: %f\n", result);
    printf("Tiempo de ejecución: %f segundos\n", time_taken);

    return 0;
}
