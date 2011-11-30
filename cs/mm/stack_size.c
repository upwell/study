#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main()
{
    struct rlimit rl;
    getrlimit(RLIMIT_STACK, &rl);

    // stack size 8MB
    printf("rlim_cur [%lld], rlim_max [%lld]\n",
            (long long) rl.rlim_cur, (long long) rl.rlim_max);

    return 0;
}
