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

int main() {
    srand(69);
    float w = get_float() * 10.0f;

    float eps = 1e-3;
    float rate = 1e-3;

    for(int i = 0; i < 1000; i++) {
        float c = cost(w);
        float dw = (cost(w + eps) - c) / eps;
        w -= rate * dw;
        printf("cost = %f, w = %f\n", c, w);
    }
    printf("w = %f\n", w);

    return 0;
}
