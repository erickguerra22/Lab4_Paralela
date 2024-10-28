#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

bool isNumber(const char *str) {
    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

// Función a integrar: x^2
double f(double x) {
    return pow(x, 2);
}

int main(int argc, char *argv[]) {
    double a = 5.0, b = 25.0;
    int n = 10000;
    double result, total_sum;
    
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        if (argc > 1) {
            if (isNumber(argv[1])) {
                a = strtod(argv[1], NULL);
            } else {
                printf("Error: el valor para 'a' no es válido.\n");
                MPI_Finalize();
                return 1;
            }
            if (argc > 2 && isNumber(argv[2])) {
                b = strtod(argv[2], NULL);
            } else {
                printf("Error: el valor para 'b' no es válido.\n");
                MPI_Finalize();
                return 1;
            }
            if (argc > 3 && isNumber(argv[3])) {
                n = atoi(argv[3]);
            } else if (argc > 3) {
                printf("Error: el valor para 'n' no es válido. Utilizando valor por defecto.\n");
                n = 10000;
            }
        }
    }

    // Broadcast de a, b y n a todos los procesos
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    double dx = (b - a) / n;
    int local_n = n / size; // Número de subintervalos por proceso
    double local_sum = 0.0;

    // Medir el tiempo de ejecución
    double start_time = MPI_Wtime();
    
    // Calcular la suma local de áreas
    for (int i = rank * local_n; i < (rank + 1) * local_n; i++) {
        double x = a + i * dx;
        local_sum += f(x) * dx;
    }

    // Reducción de las sumas locales al proceso maestro
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Solo el proceso maestro imprime el resultado
    if (rank == 0) {
        double end_time = MPI_Wtime();
        printf("Resultado aproximado de la integral x^2: %f\n", total_sum);
        printf("Tiempo de ejecución: %f segundos\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
