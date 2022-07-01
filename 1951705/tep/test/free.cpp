#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
int main()
{
    const char *tep = "1234";
    void *p = (void *)tep;
    cout << p << endl;

    return 0;
}