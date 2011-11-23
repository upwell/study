#include "hiredis/hiredis.h"

// return the ms since 1970
long long getCurrentTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);

    return (now.tv_sec*1000*1000 + now.tv_usec);
}

//typedef struct redisReply {
//    int type; /* REDIS_REPLY_* */
//    long long integer; /* The integer when type is REDIS_REPLY_INTEGER */
//    int len; /* Length of string */
//    char *str; /* Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
//    size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */
//    struct redisReply **element; /* elements vector for REDIS_REPLY_ARRAY */
//} redisReply;



void printReplyObject(const redisReply *reply)
{
    printf("--> Type:        %d\n", reply->type);
    printf("--> Integer:     %lld\n", reply->integer);
    printf("--> String len:  %d\n", reply->len);
    printf("--> String       [%s]\n", reply->str ? reply->str : "");
    printf("--> Element num: %d\n", reply->elements);

    return;
}

int main()
{
    redisContext *c;
    redisReply *reply;

    long i = 0, count = 1;
    long long start, current, consume;

    start = getCurrentTime();
    struct timeval timeout = { 1, 500000 };
    c = redisConnectUnixWithTimeout("/tmp/redis.sock", timeout);

    for(i = 0; i < count; i++)
    {
        int num = 1;
        reply = (redisReply*) redisCommand(c, "SET a %d", num);
        if(reply)
        {
            printf("%lld\n", reply->integer);
            freeReplyObject(reply);
        }
        reply = (redisReply*) redisCommand(c, "GET a");
        if(reply)
        {
            printReplyObject(reply);
            freeReplyObject(reply);
        }
    }

    consume = getCurrentTime() - start;

    printf("average handle time (us) : [%lld]\n", consume/count);

    return 0;
}
