#include <stdio.h>
#include <string.h>
#include<stdio.h>
main()
{
    char* ptr;
    if ((ptr = (char*)malloc(0)) == NULL)
        puts("Got a null pointer");
    else
        puts("Got a valid pointer");
    return 0;

}

