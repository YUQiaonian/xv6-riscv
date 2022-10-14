#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int* arr, int len){
    if(len == 0){
        return;
    }
    int p[2];
    pipe(p);
    if(fork() == 0){
        //child for feed data
        close(p[0]);
        for(int i = 0; i < len; i++){
            write(p[1], &arr[i], 4);
        }
        wait(0);
    }
    else{
        //parent to read and judge prime
        close(p[1]);
        int prime;
        read(p[0], &prime, 4);
        fprintf(1, "prime %d\n", prime);
        int n;
        int cnt = 0;
        while(read(p[0], &n, 4) == 4){
            //fprintf(1,"%d\n",n);
            if(n % prime != 0){
                *(arr++) = n;
                cnt++;
                //fprintf(1, "cnt = %d\n", cnt);
            }
        }
        //fprintf(1, "end cnt = %d\n", cnt);
        primes(arr - cnt, cnt);
        return;
    }
    wait(0);
    return;
}


int main(int argc, char* argv[]){
    int arr[34];
    for(int i = 2; i <= 35 ; i++){
        arr[i-2] = i;
    }
    printf("start primes!\n");
    primes(arr, 34);
    return 0;
}