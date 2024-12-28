#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    float or_w1;
    float or_w2;
    float or_b;
    float nand_w1;
    float nand_w2;
    float nand_b;
    float and_w1;
    float and_w2;
    float and_b;
} Xor;

typedef float sample[3];

sample xor_train[] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
};

sample or_train[] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 1},
};

sample and_train[] = {
    {0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 1},
};

sample nand_train[] = {
    {0, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
};

sample nor_train[] = {
    {0, 0, 1},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
};

sample *train = nand_train;
size_t train_count = 4;

float sigmoidf(float x) {
    return 1.f / (1.f + expf(-x));
}

float forward(Xor m, float x1, float x2) {
    float a = sigmoidf(m.or_w1*x1 + m.or_w2*x2 + m.or_b);
    float b = sigmoidf(m.nand_w1*x1 + m.nand_w2*x2 + m.nand_b);
    return sigmoidf(a*m.and_w1 + b*m.and_w2 + m.and_b);
}

float cost(Xor m) {
    float result = 0.0f;
    for (size_t i = 0; i < train_count; ++i) {
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = forward(m, x1, x2);
        float d = y - train[i][2];
        result += d*d;
    }
    result /= train_count;
    return result;
}

float rand_float() {
    return (float)rand() / (float)RAND_MAX;
}

Xor rand_xor() {
    Xor m;
    m.or_w1 = rand_float();
    m.or_w2 = rand_float();
    m.or_b = rand_float();
    m.nand_w1 = rand_float();
    m.nand_w2 = rand_float();
    m.nand_b = rand_float();
    m.and_w1 = rand_float();
    m.and_w2 = rand_float();
    m.and_b = rand_float();
    return m;
}

void print_xor(Xor m) {
    printf("m.or_w1 = %f\n", m.or_w1);
    printf("m.or_w2 = %f\n", m.or_w2);
    printf("m.or_b = %f\n", m.or_w2);
    printf("m.nand_w1 = %f\n", m.nand_w1);
    printf("m.nand_w2 == %f\n", m.nand_w2);
    printf("m.nand_b == %f\n", m.nand_b);
    printf("m.and_w1 == %f\n", m.and_w1);
    printf("m.and_w2 == %f\n", m.and_w2);
    printf("m.and_b == %f\n", m.and_b);
}



Xor finite_diff(Xor m, float eps) {
    Xor g;
    float c = cost(m);
    float saved;

    saved = m.or_w1;
    m.or_w1 += eps;
    g.or_w1 = (cost(m) - c) / eps;
    m.or_w1 = saved;

    saved = m.or_w2;
    m.or_w2 += eps;
    g.or_w2 = (cost(m) - c) / eps;
    m.or_w2 = saved;

    saved = m.or_b;
    m.or_b += eps;
    g.or_b = (cost(m) - c) / eps;
    m.or_b = saved;

    saved = m.nand_w1;
    m.nand_w1 += eps;
    g.nand_w1 = (cost(m) - c) / eps;
    m.nand_w1 = saved;

    saved = m.nand_w2;
    m.nand_w2 += eps;
    g.nand_w2 = (cost(m) - c) / eps;
    m.nand_w2 = saved;

    saved = m.nand_b;
    m.nand_b += eps;
    g.nand_b = (cost(m) - c) / eps;
    m.nand_b = saved;

    saved = m.and_w1;
    m.and_w1 += eps;
    g.and_w1 = (cost(m) - c) / eps;
    m.and_w1 = saved;

    saved = m.and_w2;
    m.and_w2 += eps;
    g.and_w2 = (cost(m) - c) / eps;
    m.and_w2 = saved;

    saved = m.and_b;
    m.and_b += eps;
    g.and_b = (cost(m) - c) / eps;
    m.and_b = saved;

    return g;
}

Xor learn(Xor m, Xor g, float rate) {
    m.or_w1 -= g.or_w1 * rate;
    m.or_w2 -= g.or_w2 * rate;
    m.or_b -= g.or_b * rate;
    m.nand_w1 -= g.nand_w1 * rate;
    m.nand_w2 -= g.nand_w2 * rate;
    m.nand_b -= g.nand_b * rate;
    m.and_w1 -= g.and_w1 * rate;
    m.and_w2 -= g.and_w2 * rate;
    m.and_b -= g.and_b * rate;
    return m;
}

int main() {
    srand(time(0));
    float eps = 1e-1;
    float rate = 1e-1;

    Xor m = rand_xor();

    printf("%f\n", cost(m));
    for (size_t i = 0; i < 100 * 1000; i++) {
        Xor g = finite_diff(m, eps);
        m = learn(m, g, rate);
        /* printf("%f\n", cost(m)); */
    }
    printf("cost = %f\n", cost(m));

    printf("OR---------------\n");
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("%zu | %zu = %f\n", i, j, sigmoidf(m.or_w1*i + m.or_w2*j + m.or_b));
        };
    }

    printf("NAND---------------\n");
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("~(%zu & %zu) = %f\n", i, j, sigmoidf(m.nand_w1*i + m.nand_w2*j + m.nand_b));
        };
    }

    printf("AND---------------\n");
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("%zu & %zu = %f\n", i, j, sigmoidf(m.and_w1*i + m.and_w2*j + m.and_b));
        };
    }

    printf("---------------\n");
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("%zu ^ %zu = %f\n", i, j, forward(m, i, j));
        };
    }

    return 0;
}


