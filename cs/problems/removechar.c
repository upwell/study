#include <stdio.h>

char *removechar(char *str, char c)
{
    int i = 0, num = 0;

    if(NULL == str)
        return NULL;

    while(*(str+i))
    {
        if(*(str+i) == c)
            num++;
        else if(num)
        {
            *(str+i-num) = *(str+i);
        }
        i++;
    }
    *(str+i-num) = 0;

    return str;
}

int main()
{
    char str[] = "abbbcefbbghbbbbbb";
    char c = 'b';

    printf("%s\n", str);

    if(removechar(str, c))
        printf("%s\n", str);
    else 
        return -1;

    return 0;
}
