#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void helper_function(int start, int end, char *str)
{
        char temp;
        if (start >= end)
                return;
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        helper_function(start+1, end-1, str);
}
        

void reverse_string(char* s, int sSize)
{
        helper_function(0, sSize-1, s);
}

void main()
{
        char data[] = "hello";
        printf("Original string: %s\n", data);
        reverse_string(data, 5);
        printf("Altered string: %s\n", data);
}
