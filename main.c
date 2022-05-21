#include <stdio.h>
#include "prompt.h"
#include "parse.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>


int main() {
	char home[503],his[1000][1003];
	int i=0;
	getcwd(&home,503);
	while(1) {
		prompt(home);
		char command[1000006];
		char copy[1000006],cp[1000006];
		fgets(command, 1000006, stdin);
		command[strlen(command)-1] = '\0';
		strcpy(copy,command);
		if(i<1000)
		{
			sprintf(his[i], "%d ", i);
			strcpy(his[i], copy);
			i++;
		}
		else
		{
			int j = i%1000;
			memset(his[j],0, strlen(his[j]));
			sprintf(his[j], "%d ", i);
			strcpy(his[j], copy);
			i++;
		}
		char **ganne = parsing(command);
		if (strcmp(ganne[0], "pinfo")==0)
		{
			char proc[503];
			if(ganne[1])
			{
				sprintf(proc, "/proc/%s/stat",ganne[1]);
				int fd = open(proc, O_RDONLY);
				if(fd!=-1) {
					lseek(fd, 0, SEEK_SET);
					char temp[501];
					read(fd, temp, sizeof(temp));
					char **status = parsing(temp);
					printf("pid -- %s\n",ganne[1]);
					printf("Process status -- {%s}\n", status[2]);
					printf("memory -- %s\n", status[22]);
					close(fd);
					int len = strlen(proc);
					proc[len - 4] = 'e';
					proc[len - 3] = 'x';
					proc[len - 2] = 'e';
					proc[len - 1] = '\0';
					char tmp[201];
					readlink(proc, tmp, sizeof(tmp));
					printf("Executable path -- %s\n", tmp);
					close(fd);
				}
			}
			else
			{
				pid_t pinfo = getpid();
				printf("pid -- %ld\n", (long)pinfo);
				sprintf(proc, "/proc/%ld/stat", (long)pinfo);
				//printf("%s",proc);
				int fd = open(proc, O_RDONLY);
				if(fd!=-1)
				{
					lseek(fd, 0, SEEK_SET);
					char temp[501];
					read(fd,temp,sizeof(temp));
					char **status = parsing(temp);
					printf("Process status -- {%s}\n",status[2]);
					printf("memory -- %s\n",status[22]);
					int len = strlen(proc);
					proc[len-4] = 'e';
					proc[len-3] = 'x';
					proc[len-2] = 'e';
					proc[len-1] = '\0';
					char tmp[201];
					readlink(proc,tmp,sizeof(tmp));
					printf("Executable path -- %s\n",tmp);
					close(fd);
				}
			}
		}
		if(strcmp(ganne[0], "history")==0)
		{
			int j = i%1000;
			if (i>=1000) {
				j++;
				for (; j < 1000; j++) {
					printf("%s\n", his[j]);
				}
				j = i % 1000;
				for (int k = 0; k <= j; ++k) {
					printf("%s\n", his[k]);
				}
			}
			else
			{
				for (int k = 0; k < j; k++) {
					printf("%s\n", his[k]);
				}
			}
		}
		if(strcmp(ganne[0] , "cd") == 0) {
			if(ganne[1] && ganne[1][0]!='~')
			{
				chdir(ganne[1]);
			}
			else if(ganne[1][0]=='~') {
				chdir(home);
			}
		}
		if(strcmp(ganne[0], "pwd")==0)
		{
			char tmp[503];
			getcwd(&tmp, 503);
			printf("%s\n",tmp);
		}
		if(strcmp(ganne[0], "echo")==0)
		{
			int i=1,j=0;
			while(ganne[i]) {
				if(ganne[i][0]!= '"') {
					printf("%s ", ganne[i]);
					i++;
				}
				else
				{
					while (copy[j]!='"')
						j++;
					j++;
					int K=0,CNT=1;
					while(copy[j]!='"')
					{
						if(copy[j]==" " && copy[j-1]!=" ")
							CNT++;
						cp[K]=copy[j];
						j++;
						K++;
					}
					j++;
					cp[K+1]='\0';
					CNT++;
					i+=CNT;
					printf("%s ", cp);
				}
			}
			printf("\n");
		}
		if (strcmp(ganne[0], "ls")==0)
		{
			DIR *fd = opendir(".");
			struct dirent *DIRECT = readdir(fd);
			if(ganne[1]) {
				if(strcmp(ganne[1], "-a")==0) {
					while (DIRECT != NULL) {
						printf("%s ", DIRECT->d_name);
						DIRECT = readdir(fd);
					}
					printf("\n");
				}
				else if(strcmp(ganne[1], ".")==0)
				{
					while(DIRECT != NULL)
					{
						if(DIRECT->d_name[0]!= '.')
							printf("%s ", DIRECT->d_name);
						DIRECT = readdir(fd);
					}
					printf("\n");
				}
				else if(strcmp(ganne[1], "..")==0)
				{
					fd = opendir("..");
					DIRECT = readdir(fd);
					while(DIRECT != NULL)
					{
						if(DIRECT->d_name[0]!= '.')
							printf("%s ", DIRECT->d_name);
						DIRECT = readdir(fd);
					}
					printf("\n");
				}
				else if(strcmp(ganne[1], "~")==0)
				{
					fd = opendir(home);
					DIRECT = readdir(fd);
					while(DIRECT != NULL)
					{
						if(DIRECT->d_name[0]!= '.')
							printf("%s ", DIRECT->d_name);
						DIRECT = readdir(fd);
					}
					printf("\n");
				}
				else if((strcmp(ganne[1], "-la")==0 || strcmp(ganne[1], "-al")==0) || (strcmp(ganne[1], "-l")==0 && ganne[2]))
				{
					fd = opendir(".");
					DIRECT = readdir(fd);
					while(DIRECT != NULL)
					{
						struct stat duck;
						int t = stat(DIRECT->d_name, &duck);
						char *mode = malloc(sizeof(char) * 10);
						if(stat(DIRECT->d_name, &duck)==0)
						{
							mode_t perm = duck.st_mode;
							mode[0] = (perm & S_IRUSR) ? 'r' : '-';
							mode[1] = (perm & S_IWUSR) ? 'w' : '-';
							mode[2] = (perm & S_IXUSR) ? 'x' : '-';
							mode[3] = (perm & S_IRGRP) ? 'r' : '-';
							mode[4] = (perm & S_IWGRP) ? 'w' : '-';
							mode[5] = (perm & S_IXGRP) ? 'x' : '-';
							mode[6] = (perm & S_IROTH) ? 'r' : '-';
							mode[7] = (perm & S_IWOTH) ? 'w' : '-';
							mode[8] = (perm & S_IXOTH) ? 'x' : '-';
							mode[9] = '\0';
							printf("%s ", mode); //1st column
							nlink_t number = duck.st_nlink;
							printf("%ld ", (long) number); //2nd column
							uid_t user = duck.st_uid;
							struct passwd *us = getpwuid(user);
							printf("%s ", us->pw_name);
							gid_t grp = duck.st_gid;
							struct passwd *group = getgrgid(grp);
							printf("%s ", group->pw_name);
							printf("%lld ", (long long) duck.st_size); //4th column
							char *time = ctime(&duck.st_ctime);
							int len = strlen(time);
							time[len-1]= '\0';
							printf("%s ", time); //5th column
							printf("%s ", DIRECT->d_name); //Last column
						}
						printf("\n");
						DIRECT = readdir(fd);
					}
				}
				else if (strcmp(ganne[1], "-l")==0)
				{
					fd = opendir(".");
					DIRECT = readdir(fd);
					while(DIRECT != NULL)
					{
						if(DIRECT->d_name[0] != '.') {
							struct stat duck;
							int t = stat(DIRECT->d_name, &duck);
							char *mode = malloc(sizeof(char) * 10);
							if(stat(DIRECT->d_name, &duck)==0)
							{
								mode_t perm = duck.st_mode;
								mode[0] = (perm & S_IRUSR) ? 'r' : '-';
								mode[1] = (perm & S_IWUSR) ? 'w' : '-';
								mode[2] = (perm & S_IXUSR) ? 'x' : '-';
								mode[3] = (perm & S_IRGRP) ? 'r' : '-';
								mode[4] = (perm & S_IWGRP) ? 'w' : '-';
								mode[5] = (perm & S_IXGRP) ? 'x' : '-';
								mode[6] = (perm & S_IROTH) ? 'r' : '-';
								mode[7] = (perm & S_IWOTH) ? 'w' : '-';
								mode[8] = (perm & S_IXOTH) ? 'x' : '-';
								mode[9] = '\0';
								printf("%s ", mode); //1st column
								nlink_t number = duck.st_nlink;
								printf("%ld ", (long) number); //2nd column
								uid_t user = duck.st_uid;
								struct passwd *us = getpwuid(user);
								printf("%s ", us->pw_name);
								gid_t grp = duck.st_gid;
								struct passwd *group = getgrgid(grp);
								printf("%s ", group->pw_name);
								printf("%lld ", (long long) duck.st_size); //4th column
								char *time = ctime(&duck.st_ctime);
								int len = strlen(time);
								time[len-1]= '\0';
								printf("%s ", time); //5th column
								printf("%s ", DIRECT->d_name); //Last column
							}
							printf("\n");
						}
						DIRECT = readdir(fd);
					}
				}
				else
				{
					fd = opendir(ganne[1]);
					DIRECT = readdir(fd);
					while(DIRECT != NULL)
					{
						if(DIRECT->d_name[0]!= '.')
							printf("%s ", DIRECT->d_name);
						DIRECT = readdir(fd);
					}
					printf("\n");
				}
			}
			else
			{
				while(DIRECT != NULL)
				{
					if(DIRECT->d_name[0]!= '.')
						printf("%s ", DIRECT->d_name);
					DIRECT = readdir(fd);
				}
				printf("\n");
			}
		}
		if(strcmp(ganne[0], "quit")==0 || strcmp(ganne[0], "exit")==0)
		{
			char abhi[]="\n\t***\tExitted Succesfully\t***\n\n";
			printf("%s",abhi);
			return 0;
		}
		else
		{
		}
	}
	return 0;
}
