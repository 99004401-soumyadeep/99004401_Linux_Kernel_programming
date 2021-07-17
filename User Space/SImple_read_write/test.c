#include <stdio.h>
#include <unistd.h>
#include<fcntl.h>



int main()
{
    int fd, nbytes;



    fd=open("/dev/psample0", O_RDWR);
    if(fd<0)
    {
        perror("open");
    }
    char str[]="abcdxyz";
    nbytes=write(fd,str,7);
    if(nbytes<0)
    {
        perror("write");
    }
    char buf[64];
    int maxlen=64;
    nbytes=read(fd,buf,maxlen);
    if(nbytes<0)
    {
        perror("write");
    }
    write(1,buf,maxlen); //(or) buf[nbytes]='\0';
    puts(buf);
    close(fd);
    return 0;
