#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void myfind(char* path, char* filename){
    int fd;
    char buf[512], *p;
    struct stat st;
    struct dirent de;
    fd = open(path, 0);

    if(fd < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;        
    }                           
 
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(strcmp(de.name, filename) == 0){
            fprintf(1, "%s\n", buf);
        }
        if(stat(buf, &st) < 0){
            fprintf(2, "stat error\n");
            return;
        }
        switch(st.type){
            case T_DEVICE:
            case T_FILE:
            break;
            case T_DIR:
                myfind(buf, filename);
            break;            
        }
    }
    close(fd);
}

int main(int argc, char* argv[]){
    if(argc < 3){
        fprintf(2, "please enter path and file: \"find path file\" \n");
        exit(1);
    }
    myfind(argv[1], argv[2]);
    exit(0);
}