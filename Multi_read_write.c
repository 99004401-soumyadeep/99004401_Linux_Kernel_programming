#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main() {
  int fd, nbytes;
  fd = open("/dev/psample", O_RDWR);
  if (fd < 0) {
    perror("open");
  }
  char str[] = "abcdxyz";
  nbytes = write(fd, str, 7);
  printf("\n%d\n", nbytes);
  if (nbytes < 0) {
    perror("write");
  }
  char rbuf[64];
  int maxlen = 64;
  while (1) {
    nbytes = read(fd, rbuf, maxlen);
    printf("\n%d\n", nbytes);
    if (nbytes == 0)
      break;
  }
  nbytes = read(fd, rbuf, maxlen);
  printf("\n%d\n", nbytes);

 

 

 

  if (nbytes < 0) {
    perror("write");
  }
  write(1, str, maxlen); //(or) buf[nbytes]='\0';
  puts(rbuf);
  close(fd);
  return 0;
}
