#include <stdio.h>
#include <stdlib.h>

int compare(const char* str, const char* ptn)
{
    int i, j;
    i = j = 0;

    int is_found = 0;

    while(str[i] && ptn[j])
    {
        if(ptn[j] == '*')
        {
            j++;
            is_found = 1;
            continue;
        } else
        {
            if(is_found)
            {
                if(str[i] != ptn[j])
                {
                    i++;
                }
                else
                {
                    is_found = 0;
                    i++;
                    j++;
                }
                continue;
            }
        }

        if(str[i] != ptn[j])
            return 0;
        i++;
        j++;
    }

    if(!str[i] && !ptn[j])
        return 1;

    if(!str[i])
    {
        while(ptn[j])
        {
            if(ptn[j] != '*')
                return 0;
        }
        return 1;
    }

    if(!ptn[j] && is_found)
        return 1;
    return 0;
}

int main()
{
    char str[] = "abcdefghij";
    char pat[] = "a*ij";

    int res = compare(str, pat);
    printf("string: %s\npattern: %s\n", str, pat);
    if(res)
        printf("matched\n");
    else
        printf("not matched\n");

    return 0;
}
