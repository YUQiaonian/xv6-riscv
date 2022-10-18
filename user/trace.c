#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]){
	char* targs[MAXARG];
	for(int i = 2; i < argc; i++){
		targs[i - 2] = argv[i];
	}
	int mask = atoi(argv[1]);
	trace(mask);
	exec(targs[0], targs);
	return 0;
}