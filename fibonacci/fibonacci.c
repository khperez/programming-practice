#include <stdio.h>
#include <stdlib.h>

int fibonacci(int n)
{
        int result = 0;
        int n_2 = 0;
        int n_1 = 1;
        if (n == 0 || n == 1)
                return n;
        for (int i = 1; i < n; i++) {
                result = n_2 + n_1;
                n_2 = n_1;
                n_1 = result;
        } 
        return result; 
}

int main(int argc, char **argv)
{
        int val;
        int fib;
        if (argc > 1)
                val = atoi(argv[1]);
        else
                printf("ERROR: not enough arguments supplied\n");
        fib = fibonacci(val);
        printf("value: %d\n", fib);
}

