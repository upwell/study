/*
 * there are three types of coins:
 * - 1 cent
 * - 3 cents
 * - 5 cents
 *
 * In order to make <total> number of
 * cents, how many coins at least are
 * required?
 *
 */

#include <iostream>
#include <limits>

#include <stdlib.h>
#include <stdint.h>

using namespace std;

static int coins[] = { 1, 3, 5 };

int dp(int total)
{
    if(total == 0)
    {
        return 0;
    }

    int len = sizeof(coins) / sizeof(coins[0]);
    int min = numeric_limits<int32_t>::max();
    int val;

    for(int i = 0; i < len; i++)
    {
        if(total >= coins[i])
        {
            val = dp(total - coins[i]) + 1;

            if(val < min)
                min = val;
        }
    }

    return min;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << argv[0] << " <total> " << endl;
        return 1;
    }

    cout << dp(atoi(argv[1])) << endl;;

    return 0;
}

