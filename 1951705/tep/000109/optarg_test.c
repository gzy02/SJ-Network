#include "Myoptarg.h"
#include <stdio.h>
int main(int argc, char **argv)
{
    Myoptarg tep;
    Set_Config(argc, argv, &tep);
    Print_Config(&tep);
    return 0;
}
