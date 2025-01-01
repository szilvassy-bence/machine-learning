#!/bin/bash

set -xe
gcc -Wall -Wextra -o xor xor.c -lm
gcc -Wall -Wextra -o adder adder.c -lm
