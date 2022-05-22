
#define __ENABLE_MEMLEAK_CHECK__
#include "mleak.h"




void test_mleak()
{
    const size_t nPoint = 5;
    void *p[nPoint] = { 0 };

    for(size_t i = 0; i < nPoint; i++)
    {
        p[i] = malloc((i + 1) * 8);
    }

    for(size_t i = 0; i < nPoint - 1; i++)
    {
        free(p[i]);
    }
}

int main()
{
    
    test_mleak();


    return 0;
}