#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

char whitespace[] = " \t\r\n\v";

int getcmd(char *buf, int nbuf)
{
  memset(buf, 0, nbuf);
  gets(buf, nbuf); //gets - get a line from a stream
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

//echo hello too | xargs echo bye
//the input is "hello too \0"
//we want get more args like "hello\0too\0"
//hello too -> there are two arg tokens "hello\0" "too\0"
int gettoken(char** ps, char* es, char** q, char** eq){
    char *s;
    int ret;

    s = *ps;
    while(s < es && strchr(whitespace, *s))
        s++; //ignore start whitespaces
    if(q)
        *q = s;
    ret = *s;
    switch(*s){
        case 0:
            break;
        default:
            ret = 'a';
            while(s < es && !strchr(whitespace, *s))
                s++; //go to the end of one token
            break;
    }
    if(eq)
        *eq = s;
    
    while(s < es && strchr(whitespace, *s))
        s++;
    *ps = s;  //ignore white spaces for next token parse
    return ret;
}

int main(int argc, char* argv[]){
    char* xargs[MAXARG];
    for(int i = 1; i < argc; i++){
        xargs[i-1] = argv[i];
    }

    static char buf[MAXARG][100]; //think why add static here?
    int i = 0;
    int j = argc-1;
    char *q, *eq;

    while(getcmd(buf[i], sizeof(buf[i])) == 0){
        char* s = &buf[i][0];    //for start of cmd buffer
        char* es = s + strlen(s); //for end of cmd buffer
        while(gettoken(&s, es, &q, &eq) != 0){
            //set end to 0
            xargs[j] = q; //add input arg at end
            *eq = 0;
            j++;
        }
        i++;
    }
    // for(int i = 0; i < j; i++){
    //     fprintf(1, "exec: %s\n", xargs[i]);
    // }

    if(fork() == 0){
        exec(xargs[0], xargs);
    }
    else{
        wait(0);
    }
    return 0;
}