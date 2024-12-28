#!/bin/bash

set -xe
gcc -Wall -Wextra -o main-2 main-2.c
gcc -Wall -Wextra -o gates gates.c -lm
gcc -Wall -Wextra -o xor xor.c -lm
