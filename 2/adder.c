#include <stddef.h>
#include <stdio.h>
#include <time.h>
#define NN_IMPLEMENTATION
#include "nn.h"

#define BITS 4

int main() {
    srand(time(0));
    // the amount possible numbers in a given count of bit is 1<<BITS => <<10 = 4
    // The combination of those numbers is the following
    size_t n = (1<<BITS);
    size_t rows = n*n;
    Mat ti = mat_alloc(rows, 2 * BITS);
    Mat to = mat_alloc(rows, BITS + 1);
    for (size_t i = 0; i < ti.rows; ++i) {
        size_t x = i/n;
        size_t y = i%n;
        size_t z = x + y;
        for (size_t j = 0; j < BITS; ++j) {
            // the x is shifted right by j positions and & operation on 0001
            MAT_AT(ti, i, j) = (x>>j)&1;
            MAT_AT(ti, i, j + BITS) = (y>>j)&1;
            MAT_AT(to, i, j) = (z>>j)&1;
        }
        MAT_AT(to, i, BITS) = z >= n;
    }

    MAT_PRINT(ti);
    MAT_PRINT(to);
    size_t t = ti.rows;

    float rate = 1;

    size_t arch[] = {2*BITS, 4*BITS, BITS + 1};
    NN nn = nn_alloc(arch, ARRAY_LEN(arch));
    NN g = nn_alloc(arch, ARRAY_LEN(arch));
    nn_rand(nn, 0, 1);
    printf("cost = %f\n", nn_cost(nn, ti, to));
    for (size_t i = 0; i < 10000; ++i) {
#if 1
        nn_backprop(nn, g, ti, to);
#else
        float eps = 1e-1;
        nn_finite_diff(nn, g, eps, ti, to);
#endif
        nn_learn(nn, g, rate);
        printf("cost = %f\n", nn_cost(nn, ti, to));
    }

    size_t fails = 0;
    for (size_t x = 0; x < n; ++x) {
        for (size_t y = 0; y < n; ++y) {
            size_t z = x + y;
            for (size_t j = 0; j < BITS; ++j) {
                MAT_AT(NN_INPUT(nn), 0, j) = (x>>j)&1;
                MAT_AT(NN_INPUT(nn), 0, j + BITS) = (y>>j)&1;
            }
            nn_forward(nn);
            if (MAT_AT(NN_OUTPUT(nn), 0, BITS) > 0.5f) {
                if (z < n) {
                    printf("%zu + %zu = (OVERFLOW|%zu)\n", x, y, z);
                    fails += 1;
                }
            } else {
                size_t a = 0;
                for (size_t j = 0; j < BITS; ++j) {
                    size_t bit = MAT_AT(NN_OUTPUT(nn), 0, j) > 0.5f;
                    a |= bit<<j;
                }
                if (z != a) {
                    printf("%zu + %zu = (%zu<>%zu)\n", x, y, z, a);
                    fails += 1;
                }
            }
        }
    }
    if (fails == 0) {
        printf("OK\n");
    }
    return 0;
}
