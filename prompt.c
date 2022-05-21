//
// Created by abhinav on 01/09/20.
//
#include <stdio.h>
#include <sys/utsname.h>
#include "prompt.h"
#include <unistd.h>
#include <string.h>
#include "parse.h"

void prompt(char *home)
{
    struct utsname buf;
    int check = uname(&buf);
    char buff[203];
    getcwd(&buff, 203);
    char **duck = parsing(buff);
    int len = strlen(home),leng=strlen(buff);
    if(leng<len)
    {
        printf("<%s@",buf.nodename);
        printf("%s:%s> ",buf.sysname, buff);
    }
    else {
        if(strncmp(home,buff,len)!=0) {
            printf("<%s@", buf.nodename);
            printf("%s:%s> ", buf.sysname, buff);
        }
        else
        {
            printf("<%s@%s~", buf.nodename, buf.sysname);
            for(int i=len;i<leng;i++)
            {
                printf("%c",buff[i]);
            }
            printf("> ");
        }
    }
}
