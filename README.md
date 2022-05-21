# Virtual-Terminal-Using-C
This project to build a virtual terminal using C. This mimics the working of a linux terminal.

Implemented cd,ls[all],pwd,echo,history,pinfo.

cd works fine till the code's path contains a folder with spaces in it's name.

history prints all the commands we tested. (maximum of 1000)

quit or exit - stops the execution of the shell.

MakeFile works as follows:

make all - creates a file namely shell. Running it qives you my shell.

make clean - cleans all created files.