#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    char buf[1024];
    int len;

    if (argc != 3)
    {
        printf(1, "Usage: grep_test <keyword> <filename>\n");
        exit();
    }

    len = grep_syscall(argv[1], argv[2], buf, sizeof(buf));

    if (len == -1)
    {
        printf(1, "grep_test: Keyword not found or file error.\n");
    }
    else
    {
        printf(1, "grep_test: ");
        printf(1, "%s\n", buf);
    }

    exit();
}