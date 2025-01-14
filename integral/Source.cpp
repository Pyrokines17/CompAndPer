#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926536

double Integral(double* f, double step, long int NUMPOINT) {
    double value = 0;

    for (int i = 0; i < NUMPOINT; i++) {
        value += f[i];
    }

    value *= step;

    return value;
}

int main(int argc, char* argv[]) {
    double* f;
    double step, t;
    double S;
    long int NUMPOINT;
    int i;

    system("chcp 1251");
    system("cls");

    if (argc == 1) {
        printf("Error: where arguments?");
        exit(1);
    }

    NUMPOINT = atol(argv[1]);

    f = (double*)malloc(NUMPOINT * sizeof(double));

    if (f == NULL) {
        printf("-memory");
        exit(1);
    }

    printf("Количество точек = %d\n", NUMPOINT);

    step = PI / NUMPOINT; 

    printf("Величина шага = %lf\n", step);

    t = 0.0;

    for (i = 0; i < NUMPOINT; i++) {
        f[i] = sin(t) * exp(t);
        t += step;
    }

    S = Integral(f, step, NUMPOINT); 

    printf("Значение интеграла = %lf\n", S);

    return 0;
}
