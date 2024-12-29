#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int train[][2] = {
    {0, 0},
    {1, 2},
    {2, 4},
    {4, 8}
};

#define train_count (sizeof(train) / sizeof(train[0]))

float get_float() {
    return (float)rand() / (float)RAND_MAX;
}

float cost(float w) {
    float result = 0.0f;
    for (size_t i = 0; i < train_count; i++) {
        float x = train[i][0];
        float y = x * w;
        float d = y - train[i][1];
        result += d*d;
    }
    return result /= train_count;
}

float dcost(float w) {
    float result = 0.0f;
    size_t n = train_count;
    for (size_t i = 0; i < n; ++i) {
        float x = train[i][0];
        float y = train[i][1];
        result += 2 * (x * w - y) * x;
    }
    return result /= n;
}

int main() {
    srand(69);
    float w = get_float() * 10.0f;

    float rate = 1e-2;

    for (int i = 0; i < 6; i++) {
#if 1
        float eps = 1e-3;
        float c = cost(w);
        float dw = (cost(w + eps) - c) / eps;
#else
        float dw = dcost(w);
#endif
        w -= rate * dw;
        printf("cost = %f, w = %f\n", cost(w), w);
    }
    printf("w = %f\n", w);

    return 0;
}
