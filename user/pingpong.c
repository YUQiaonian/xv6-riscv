#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    int p[2];
    pipe(p);
    int pid = fork();
    // fprintf(1, "in main %d \n",pid);
    // fprintf(1, "in main get%d \n",getpid());

    if(pid == 0){
        //child
        // fprintf(1, "in child %d \n",pid);
        char c[1];
        read(p[0], c, 1);
        close(p[0]);
        fprintf(1, "%d: received ping\n", getpid());
        write(p[1], c, 1);
        close(p[1]);
        exit(0);
    }
    else{
        //parent
        // fprintf(1, "in father %d \n",pid);
        char c[1];
        c[0] = 's';
        write(p[1], c, 1);
        close(p[1]);
        read(p[0], c, 1);
        fprintf(1, "%d: received pong\n", getpid());
        close(p[0]);
        exit(0);
    }
}