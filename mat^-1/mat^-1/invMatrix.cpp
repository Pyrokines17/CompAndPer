#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <xmmintrin.h>

const int N = 512;
const int iter = 10;

float proizv(float* x, float* y) {
	float summa;
	__m128 *xx, *yy, p, s;

	xx = reinterpret_cast<__m128*>(x);
	yy = reinterpret_cast<__m128*>(y);
	s = _mm_set_ps1(0);

	for (int i = 0; i < N / 4; i++) {
		p = _mm_mul_ps(xx[i], yy[i]);
		s = _mm_add_ps(s, p);
	}

	p = _mm_movehl_ps(p, s);
	s = _mm_add_ps(s, p);
	p = _mm_shuffle_ps(s, s, 1);
	s = _mm_add_ss(s, p);
	_mm_store_ss(&summa, s);

	return summa;
}

void dot(float* A, float* B, float* C) {
	float* mas;
	mas = new float[N * N];

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			mas[j * N + i] = B[i * N + j];
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i * N + j] = proizv(&A[i * N], &mas[j * N]);
		}
	}
}

void add(float* A, float* B, float* C) {
	__m128 p;

	for (int i = 0; i < N; i++) {
		for (int k = 0; k < N / 4; k++) {
			__m128 *xx, *yy;

			xx = reinterpret_cast<__m128*>(&A[i * N]);
			yy = reinterpret_cast<__m128*>(&B[i * N]);
			p = _mm_add_ps(xx[k], yy[k]);
			_mm_store_ps(&C[i * N + k * 4], p);
		}
	}
}

void sub(float* A, float* B, float* C) {
	__m128 p;

	for (int i = 0; i < N; i++) {
		for (int k = 0; k < N / 4; k++) {
			__m128 *xx, *yy;

			xx = reinterpret_cast<__m128*>(&A[i * N]);
			yy = reinterpret_cast<__m128*>(&B[i * N]);
			p = _mm_sub_ps(xx[k], yy[k]);
			_mm_store_ps(&C[i * N + k * 4], p);
		}
	}
}

double S_SSE(float* mas, float* BB) {
	float *I, *B, *R, *C, *sib, max1, maxst, summa;
	clock_t start1;

	I = new float[N * N];
	B = new float[N * N];
	R = new float[N * N];
	C = new float[N * N];
	sib = new float[N * N];

	max1 = 0;
	summa = 0;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			summa += static_cast<float>(fabs(mas[i * N + j]));
		}
		if (summa > max1) {
			max1 = summa;
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
			B[i * N + j] = mas[j * N + i] / (max1 * maxst);
			B[i * N + j] = mas[j * N + i] / (max1 * maxst);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) {
				I[i * N + j] = 512;
			}
			else {
				I[i * N + j] = 1;
			}
		}
	}

	dot(mas, B, C);
	sub(I, C, R);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			BB[i * N + j] = I[i * N + j];
			sib[i * N + j] = I[i * N + j];
		}
	}

	start1 = clock();

	for (int s = 1; s <= iter; s++) {
		dot(sib, R, C);

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				sib[i * N + j] = C[i * N + j];
			}
		}

		add(BB, sib, BB);
	}

	dot(BB, B, C);
	start1 = clock() - start1;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			BB[i * N + j] = C[i * N + j];
		}
	}

	return static_cast<double>(start1) / (CLK_TCK * iter);
}

void umnogenie(float* A, float* B, float* C) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i * N + j] = 0;
			for (int k = 0; k < N; k++) {
				C[i * N + j] += A[i * N + k] * B[k * N + j];
			}
		}
	}
}

void suma(float* A, float* B, float* C) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i * N + j] = A[i * N + j] + B[i * N + j];
		}
	}
}

void raznost(float* A, float* B, float* C) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i * N + j] = A[i * N + j] - B[i * N + j];
		}
	}
}

double flip(float* mas, float* BB) {
	float *I, *B, *R, *C, *sib, max, maxst, summa;
	clock_t start;

	I = new float[N * N];
	B = new float[N * N];
	R = new float[N * N];
	C = new float[N * N];
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
			B[i * N + j] = mas[j * N + i] / (max * maxst);
			B[i * N + j] = mas[j * N + i] / (max * maxst);
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

	umnogenie(mas, B, C);
	raznost(I, C, R);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			BB[i * N + j] = I[i * N + j];
			sib[i * N + j] = I[i * N + j];
		}
	}

	start = clock();

	for (int s = 1; s <= iter; s++) {
		umnogenie(sib, R, C);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				sib[i * N + j] = C[i * N + j];
			}
		}
		suma(BB, sib, BB);
	}

	umnogenie(BB, B, C);
	start = clock() - start;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			BB[i * N + j] = C[i * N + j];
		}
	}

	return static_cast<double>(start) / (CLK_TCK * iter);
}

int main() {
	double t, t1, t2;
	clock_t start;
	float *m1, *tech;

	m1 = new float[N * N];
	tech = new float[N * N];

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) {
				m1[i * N + j] = 512;
			}
			else {
				m1[i * N + j] = 1;
			}
		}
	}

	printf("Vremya bez SSE: \n");

	start = clock();
	t = flip(m1, tech);
	start = clock() - start;
	t1 = static_cast<double>(start) / CLK_TCK;

	printf("Vremya iteracii: %f \n", t);
	printf("Vremya cikla: %f \n", t1);

	printf("Vremya s SSE: \n");

	start = clock();
	t = S_SSE(m1, tech);
	start = clock() - start;
	t2 = static_cast<double>(start) / CLK_TCK;

	printf("Vremya iteracii: %f \n", t);
	printf("Vremya cikla: %f \n", t2);

	return 0;
}