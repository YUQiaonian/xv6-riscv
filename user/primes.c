#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int fd){
	int prime;
	if(read(fd, &prime, 4) != 0){
		fprintf(1, "prime %d\n", prime);
	}
	else{
		return;
	}

	int n = -1;
	int p[2];
	pipe(p);
	//fprintf(1, "fd %d p[0] %d p[1] %d\n", fd, p[0], p[1]);
	while(1){
		if(read(fd, &n, 4) == 0){
			break;
		}
		else{
			if(n%prime != 0){
				write(p[1], &n, 4);
			}
		}
	}

	if(n == -1){
		close(fd);
		close(p[0]);
		close(p[1]);
		return;
	}

	if(fork() == 0){
		//child
		close(fd);
		close(p[1]);
		primes(p[0]);
		close(p[0]);
	}
	else{
		//parent
		close(fd);
		close(p[0]);
		close(p[1]);
		wait(0);
		return;
	}
}

int main(int argc, char* argv[]){
	int p[2];
	pipe(p);
	for(int i = 2; i <= 35; i++){
		write(p[1], &i, 4);
	}

	close(p[1]);
	primes(p[0]);
	close(p[0]);
    
    return 0;
}