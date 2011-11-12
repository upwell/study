#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <sys/types.h>
#include <pwd.h>

/*
 * it seems this program keeps running ok,
 * maybe the getpwnam is safe during the signal
 * interrupts
 *
 */

void sig_handler(int signo)
{
    if(signo != SIGALRM)
    {
        printf("weired signal received [%d]\n", signo);
    }

    alarm(1);
}

int main()
{
    struct passwd *usr_pwd = NULL;

    signal(SIGALRM, sig_handler);

    alarm(1);

    while(1)
    {
        if(NULL == (usr_pwd = getpwnam("han")))
        {
            perror("getpwnam for user han error");
            return -1;
        }

        if(strncmp(usr_pwd->pw_name, "han" , 3))
        {
            fprintf(stderr, "got a wrong user [%s]\n", usr_pwd->pw_name);
            break;
        }
    }

    return 0;
}
