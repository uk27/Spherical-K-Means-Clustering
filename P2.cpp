#include <stdlib.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int main()
{
    char cmd[] = "/usr/bin/python preprocessor.py";
    printf("Doing Preprocessing...\n");
    system(cmd);
    return 0;
}
