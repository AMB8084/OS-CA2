#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
int main(int argc, char * argv[]){
    if (argc < 3){
        printf(1, "copy_file: 2 args required\n");
        exit();
    }
    char * src = argv[1];
    char * des = argv[2];
    if (copy_file(src,des) < 0){
        printf(1,"copy_file: cannot copy the file\n");
        exit();
    }
    printf(1,"%s has been copied to %s\n", src, des);
    exit();
}