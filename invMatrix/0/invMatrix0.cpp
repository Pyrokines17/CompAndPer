#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>

const int N = 512;
const int M = 10;

void mul(const float* X, const float* Y, float* Z) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Z[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                Z[i * N + k] += X[i * N + j] * Y[j * N + k];
            }
        }
    }
}

void sum(const float* X, const float* Y, float* Z) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Z[i * N + j] = X[i * N + j] + Y[i * N + j];
        }
    }
}

void sub(const float* X, const float* Y, float* Z) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Z[i * N + j] = X[i * N + j] - Y[i * N + j];
        }
    }
}

double inv(float* mas, float* res) {
    float* I, * B, * R, * Z, * sib, max, max1, summa;
    clock_t start;

    I = new float[N * N];
    B = new float[N * N];
    R = new float[N * N];
    Z = new float[N * N];
    sib = new float[N * N];

    max = 0;

    for (int i = 0; i < N; i++) {
        summa = 0;
        for (int j = 0; j < N; j++) {
            summa += static_cast<float>(std::fabs(mas[i * N + j]));
        }
        if (summa > max) {
            max = summa;
        }
    }

    max1 = 0;

    for (int j = 0; j < N; j++) {
        summa = 0;
        for (int i = 0; i < N; i++) {
            summa += static_cast<float>(std::fabs(mas[i * N + j]));
        }
        if (summa > max1) {
            max1 = summa;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            B[i * N + j] = mas[j * N + i] / (max * max1);
            B[j * N + i] = mas[i * N + j] / (max * max1);
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                I[i * N + j] = 1;
            } else {
                I[i * N + j] = 0;
            }
        }
    }

    mul(B, mas, Z);
    sub(I, Z, R);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i * N + j] = I[i * N + j];
            sib[i * N + j] = I[i * N + j];
        }
    }

    start = clock();

    for (int s = 1; s <= M; s++) {
        mul(sib, R, Z);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sib[i * N + j] = Z[i * N + j];
            }
        }

        sum(res, sib, res);
    }

    mul(res, B, Z);
    start = clock() - start;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i * N + j] = Z[i * N + j];
        }
    }

    return static_cast<double>(start) / (CLOCKS_PER_SEC * M);
}

int main() {
    double t, t1;
    clock_t start;
    float* mat, * res;

    mat = new float[N * N];
    res = new float[N * N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                mat[i* N + j] = 1;
            } else {
                mat[i * N + j] = 0;
            }
        }
    }

    std::cout << "_______________________________" << std::endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << mat[i * N + j];
        }
        std::cout << std::endl;
    }

    start = clock();
    t = inv(mat, res);
    start = clock() - start;
    t1 = static_cast<double>(start) / CLOCKS_PER_SEC;

    std::cout << "_______________________________" << std::endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << res[i * N + j];
        }
        std::cout << std::endl;
    }

    std::cout << "_______________________________" << std::endl;

    printf("Time without vectorization: \n");
    printf("Time of iteration: %f \n", t);
    printf("Time of cycle: %f \n", t1);

    std::cout << "_______________________________" << std::endl;

    return 0;
}
