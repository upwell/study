#include <stdio.h>

void genknuth(int, int);
void usage();

int main(int argc, char *argv[])
{
    int m, n;

    if(argc < 3)
    {
        usage();
        return -1;
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    if(m >= n)
    {
        usage();
        return -1;
    }

    genknuth(m, n);

    return 0;
}

void usage()
{
    printf("usage: ./a.out n m\n");
}

void genknuth(int m, int n)
{
    int i = 0;
    srand((int) time(0));
    for(i = 0; i < n; i++)
    {
        if(rand() % (n-i) < m)
        {
            printf("%d ", i);
            m--;
        }
    }

    printf("\n");
    return;
}
