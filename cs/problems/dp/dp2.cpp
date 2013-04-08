/*
 *
 * longest increasing subsequence
 *
 */

#include <iostream>

using namespace std;

int global_arr[] = { 5, 3, 4, 8, 6, 10, 5, 9 };

int dp(int *ar, int len)
{
    int *d = new int[len];

    int max = 1;

    for(int i = 0; i < len; i++)
    {
        d[i] = 1;

        for(int j = 0; j < i; j++)
        {
            if(ar[i] >= ar[j] && d[j] + 1 > d[i])
                d[i] = d[j] + 1;

            if(i == 7)
            {
                cout << j << " " << d[j] << " " << d[i] << endl;
            }
        }

        if(d[i] > max) max = d[i];
    }

    delete []d;

    return max;
}

int main()
{
    cout << dp(global_arr, sizeof(global_arr)/sizeof(global_arr[0]))
        << endl;

    return 0;
}
