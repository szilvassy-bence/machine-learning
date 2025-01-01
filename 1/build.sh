#!/bin/bash

set -xe

gcc -Wall -Wextra -o main main.c
gcc -Wall -Wextra -o gates gates.c -lm
