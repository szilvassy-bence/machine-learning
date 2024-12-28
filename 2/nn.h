#ifndef NN_H_
#define NN_H_
// Header part

#include <stddef.h>
#include <stdio.h>

#ifndef NN_MALLOC
#include <stdlib.h>
#define NN_MALLOC malloc
#endif // NN_MALLOC

#ifndef NN_ASSERT
#include <assert.h>
#define NN_ASSERT assert
#endif // NN_ASSERT

// the structure does not contain the data
// size_t is 64 bits
typedef struct {
    size_t rows;
    size_t cols;
    float *es;
} Mat;

#define MAT_AT(m, i, j) (m).es[(i)*(m).cols + (j)]

float float_rand();

// memory management here
Mat mat_alloc(size_t rows, size_t cols);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst, Mat a);
void mat_print(Mat m, const char *name);
void mat_fill(Mat m, float v);
void mat_rand(Mat m, float low, float high);
#define MAT_PRINT(m) mat_print(m, #m)

#endif // NN_H_

#ifdef NN_IMPLEMENTATION
// c part

float float_rand() {
    return (float)rand() / (float)RAND_MAX;
}

Mat mat_alloc(size_t rows, size_t cols) {
    Mat m;
    m.rows = rows;
    m.cols = cols;
    m.es = malloc(sizeof(*m.es) * rows * cols);
    assert(m.es != NULL);
    return m;
}

void mat_dot(Mat dst, Mat a, Mat b) {
    NN_ASSERT(a.cols == b.rows);
    size_t n = a.cols;
    NN_ASSERT(dst.rows == a.rows);
    NN_ASSERT(dst.cols == b.cols);

    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MAT_AT(dst, i, j) = 0;
            for (size_t k = 0; k < n; ++k) {
                MAT_AT(dst, i, j) += MAT_AT(a, i, k) * MAT_AT(b, k, j);
            }
        }
    }
}

void mat_sum(Mat dst, Mat a) {
    NN_ASSERT(dst.rows == a.rows);
    NN_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MAT_AT(dst, i, j) += MAT_AT(a, i, j);
        }
    }
}

void mat_print(Mat m, const char *name){
    printf("%s = [\n", name);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            printf("    %f ", MAT_AT(m, i, j));
        }
        printf("\n");
    }
    printf("]\n");
    (void)m;
}

void mat_rand(Mat m, float low, float high) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT_AT(m, i, j) = float_rand() * (high-low) + low;
        }
    }
}

void mat_fill(Mat m, float v) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT_AT(m, i, j) = v;
        }
    }
}

#endif // NN_IMPLEMENTATION
