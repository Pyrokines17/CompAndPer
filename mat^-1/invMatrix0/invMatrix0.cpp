#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const int N = 512;
const int M = 10;

void mul(float* X, float* Y, float* Z) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Z[i * N + j] = 0;
			for (int k = 0; k < N; k++) {
				Z[i * N + j] += X[i * N + k] * Y[k * N + j];
			}
		}
	}
}

void sum(float* X, float* Y, float* Z) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Z[i * N + j] = X[i * N + j] + Y[i * N + j];
		}
	}
}

void sub(float* X, float* Y, float* Z) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Z[i * N + j] = X[i * N + j] - Y[i * N + j];
		}
	}
}

double inv(float* mas, float* BB) {
	float* I, * Y, * R, * Z, * sib, max, maxst, summa; //(?)//
	clock_t start;

	I = new float[N * N];
	Y = new float[N * N];
	R = new float[N * N];
	Z = new float[N * N];
	sib = new float[N * N];

	max = 0;
	summa = 0;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			summa += static_cast<float>(fabs(mas[i * N + j]));
		}
		if (summa > max) {
			max = summa;
		}
	}

	maxst = 0;
	summa = 0;

	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			summa += static_cast<float>(fabs(mas[i * N + j]));
		}
		if (summa > maxst) {
			maxst = summa;
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = i; j < N; j++) {
			Y[i * N + j] = mas[j * N + i] / (max * maxst);
			Y[i * N + j] = mas[j * N + i] / (max * maxst);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) {
				I[i * N + j] = 1;
			}
			else {
				I[i * N + j] = 0;
			}
		}
	}

	mul(mas, Y, Z);
	sub(I, Z, R);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			BB[i * N + j] = I[i * N + j];
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
		sum(BB, sib, BB);
	}

	mul(BB, Y, Z);
	start = clock() - start;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			BB[i * N + j] = Z[i * N + j];
		}
	}

	return static_cast<double>(start) / (CLK_TCK * M);
}

int main() {
	double t, t1;
	clock_t start;
	float* m, * res;

	m = new float[N * N];
	res = new float[N * N];

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) {
				m[i * N + j] = 512;
			}
			else {
				m[i * N + j] = 1;
			}
		}
	}

	printf("Vremya bez SSE: \n");

	start = clock();
	t = inv(m, res);
	start = clock() - start;
	t1 = static_cast<double>(start) / CLK_TCK;

	printf("Vremya iteracii: %f \n", t);
	printf("Vremya cikla: %f \n", t1);

	return 0;
}