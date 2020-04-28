#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
//#include<linux/timekeeping.h>
int main()
{
  struct timeval tv;

  //sleep(5);
  gettimeofday(&tv, NULL);
  long sec,nsec;
  sec=tv.tv_sec;
  nsec=tv.tv_usec*1000+(tv.tv_usec / 100 + 37) % 1000;
  printf("%ld.%ld\n",sec,nsec);

  /*printf("FIFO:　%d\n", 'F' + 'I' + 'F' + 'O');
  printf("SJF:　%d\n", 'S' + 'J' + 'F');
  printf("PSJF:　%d\n", 'P' + 'S' + 'J' + 'F');
  printf("RR:　%d\n", 'R' + 'R');*/
  return 0;
}