#include <stdio.h>
#include <stdlib.h>
#include "Header.h"

#define L 64
#define sqrt_L 8
#define MK 56
#define K 48
#define ROUND 16

int ip_array[sqrt_L] = { 2, 4, 6, 8, 1, 3, 5, 7 };

int s_box[8][4][16] = { {{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
						{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
						{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
						{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}},

					   {{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
						{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
						{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
						{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}},

					   {{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
						{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
						{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
						{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}},

					   {{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
						{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
						{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
						{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}},

					   {{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
						{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
						{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
						{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}},

					   {{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
						{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
						{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
						{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}},

					   {{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
						{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
						{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
						{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}},

					   {{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
						{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
						{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
						{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}} };

int permutation_table[L / 2] = { 16,  7, 20, 21, 29, 12, 28, 17,
							   1, 15, 23, 26,  5, 18, 31, 10,
							   2,  8, 24, 14, 32, 27,  3,  9,
							  19, 13, 30,  6, 22, 11,  4, 25 };

short int bits_rotated[ROUND] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

int pc2[K] = { 14, 17, 11, 24,  1,  5,  3, 28,
			  15,  6, 21, 10, 23, 19, 12,  4,
			  26,  8, 16,  7, 27, 20, 13,  2,
			  41, 52, 31, 37, 47, 55, 30, 40,
			  51, 45, 33, 48, 44, 49, 39, 56,
			  34, 53, 46, 42, 50, 36, 29, 32 };

int pc1[MK] = { 50, 43, 36, 29, 22, 15,  8,
				1, 51, 44, 37, 30, 23, 16,
				9,  2, 52, 45, 38, 31, 24,
			   17, 10,  3, 53, 46, 39, 32,
			   56, 49, 42, 35, 28, 21, 14,
				7, 55, 48, 41, 34, 27, 20,
			   13,  6, 54, 47, 40, 33, 26,
			   19, 12,  5, 25, 18, 11,  4 };

short int dec_key[ROUND][K];

void key_gen(short int* main_key, short int* key, int index) {
	int i, j;
	short int t;
	for (j = 0; j < bits_rotated[index - 1]; j++) {
		for (i = MK / 2 - 1; i >= 0; i--) {
			t = main_key[0];
			main_key[0] = main_key[i];
			main_key[i] = t;
		}
		for (i = MK - 1; i >= MK / 2; i--) {
			t = main_key[MK / 2];
			main_key[MK / 2] = main_key[i];
			main_key[i] = t;
		}
	}
	for (i = 0; i < K; i++) {
		key[i] = main_key[pc2[i] - 1];
	}
}

void r_expand(short int* r, short int* expanded_r) {
	int i, j, index = 0;
	for (i = 0; i < L / 2; i++) {
		if (index % (K / sqrt_L) == 0) {
			j = i - 1;
			if (j < 0) {
				j += L / 2;
			}
			expanded_r[index] = r[j];
			index++;
		}

		expanded_r[index] = r[i];
		index++;

		if (index % (K / sqrt_L) == K / sqrt_L - 1) {
			j = i + 1;
			if (j >= L / 2) {
				j -= L / 2;
			}
			expanded_r[index] = r[j];
			index++;
		}
	}
}

void x_or(short int* arr, short int* key, int size, int zero) {
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == key[i]) {
			arr[i] = zero;
		}
		else {
			arr[i] = 1;
		}
	}
}

void substitution(short int* r, short int* expanded_r) {
	int row = 0, column = 0;
	int i, j, s;
	for (i = 0; i < K / 6; i++) {
		row = expanded_r[i * 6] * 2 + expanded_r[i * 6 + 5];
		column = expanded_r[i * 6 + 1] * 8 + expanded_r[i * 6 + 2] * 4 + expanded_r[i * 6 + 3] * 2 + expanded_r[i * 6 + 4];
		s = s_box[i][row][column];
		for (j = 3; j >= 0; j--) {
			r[j + i * 4] = (s % 2) * 2 - 1;
			s /= 2;
		}
	}
}

void permutation(short int* r, int size, int* table) {
	int i;
	short int temp[MK];
	for (i = 0; i < size; i++) {
		temp[i] = r[i];
	}
	for (i = 0; i < size; i++) {
		r[i] = temp[table[i] - 1];
	}
}

void des_enc_round(short int* input, short int* output, short int* key) {
	int i;
	for (i = 0; i < L / 2; i++) {
		output[i] = input[i + L / 2];
	}

	short int expanded_r[K];

	r_expand(input + L / 2, expanded_r);

	x_or(expanded_r, key, K, 0);

	substitution(output + L / 2, expanded_r);

	permutation(output + L / 2, L / 2, permutation_table);

	x_or(output + L / 2, input, L / 2, 0);
}

void ip_rev(short int* input, short int* output) {
	int i, j;
	for (i = 0; i < L; i++) {
		j = ip_array[i / sqrt_L] + (sqrt_L - 1 - i % 8) * sqrt_L - 1;
		output[j] = input[i];
	}
}

void ip(short int* input, short int* output) {
	int i, j;
	for (i = 0; i < L; i++) {
		j = ip_array[i / sqrt_L] + (sqrt_L - 1 - i % 8) * sqrt_L - 1;
		output[i] = input[j];
	}
}

void des_enc(short int* input, short int* output, short int* main_key) {
	permutation(main_key, MK, pc1);
	int i, j;
	short int key[K];

	ip(input, output);

	for (i = 1; i <= ROUND; i++) {
		for (j = 0; j < L; j++) {
			input[j] = output[j];
		}
		key_gen(main_key, key, i);
		des_enc_round(input, output, key);
	}

	j = L / 2;
	for (i = 0; i < L; i++) {
		if (j == L) {
			j = 0;
		}
		input[j] = output[i];
		j++;
	}

	ip_rev(input, output);
}

void des_dec(short int* input, short int* output, short int* main_key) {
	permutation(main_key, MK, pc1);
	int i, j;
	short int key[K];
	for (i = 0; i < ROUND; i++) {
		key_gen(main_key, key, i + 1);
		for (j = 0; j < K; j++) {
			dec_key[i][j] = key[j];
		}
	}

	ip(input, output);

	for (i = ROUND; i >= 1; i--) {
		for (j = 0; j < L; j++) {
			input[j] = output[j];
		}
		des_enc_round(input, output, dec_key[i - 1]);
	}

	j = L / 2;
	for (i = 0; i < L; i++) {
		if (j == L) {
			j = 0;
		}
		input[j] = output[i];
		j++;
	}

	ip_rev(input, output);
}
