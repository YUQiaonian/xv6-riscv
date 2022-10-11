#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(2, "sleep need an integer argument for second!\n");
        exit(1);
    }
    int sec = atoi(argv[1]);
    sleep(sec);
    exit(0);
}