#include <wchar.h>
#include <stdio.h>
#include <locale.h>

int main()
{
    wchar_t a[] = L"\\\\labfs\\abc";

    char locale[] = "en_US.UTF-8";

    setlocale(LC_ALL, locale);

    int len = wcslen(a);
    printf("len  [%d]\n", len);
    printf("size [%d]\n", sizeof(a));

    wchar_t *pos = wcschr(a+2, L'\\');
    if(pos)
    {
        *pos = L'\0';
        fwprintf(stdout, L"[%ls]\n", a);
    }
    else
        fprintf(stdout, "not found\n");

    return 0;
}
