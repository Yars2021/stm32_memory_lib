#define MAIN

#include "include.h"

int main (void)
{
    initLowLevel();

    while (1)
    {
        WriteToFlash(12567);
        Flash_ReadParams();
    }
}
