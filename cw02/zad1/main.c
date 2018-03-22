#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include "lib.h"

double timeDiff(clock_t start, clock_t end)
{
    return (double)(end -  start) / sysconf(_SC_CLK_TCK);
}

void printTime(struct tms *tmsStartTime, struct tms *tmsEndTime)
{
  printf("delta_user:%.100f\n", timeDiff(tmsStartTime->tms_utime, tmsEndTime->tms_utime));
  printf("delta_system:%.100f\n", timeDiff(tmsStartTime->tms_stime, tmsEndTime->tms_stime));
}

int main(int argc, char **argv)
{
  if(argc < 5)
  {
    printf("Too few arguments\n");
    return 1;
  }
  if(argc == 5) //generate
  {
    if(strcmp(argv[1], "generate") != 0)
      return 1;
    generate(argv[2], (int) strtol(argv[3], NULL, 10), (int) strtol(argv[4], NULL, 10));
    return 0;
  }
  if(argc == 6) //sort
  {
    if(strcmp(argv[1], "sort") != 0)
      return 1;
    struct tms *before = (struct tms*) malloc(sizeof(struct tms));
    struct tms *after = (struct tms*) malloc(sizeof(struct tms));
    if(strcmp(argv[5], "sys") == 0)
    {
      times(before);
      sort_sys(argv[2], (int) strtol(argv[3], NULL, 10), (int) strtol(argv[4], NULL, 10));
      times(after);
      printTime(before, after);
    }
    else if(strcmp(argv[5], "lib") == 0)
    {
      times(before);
      sort_lib(argv[2], (int) strtol(argv[3], NULL, 10), (int) strtol(argv[4], NULL, 10));
      times(after);
      printTime(before, after);
    }
    else
    {
      printf("Invalid argument\n");
      return 1;
    }
    free(before);
    free(after);
    return 0;
  }
  if(argc == 7) //copy
  {
    if(strcmp(argv[1], "copy") != 0)
      return 1;
    struct tms *before = (struct tms*) malloc(sizeof(struct tms));
    struct tms *after = (struct tms*) malloc(sizeof(struct tms));
    if(strcmp(argv[6], "sys") == 0)
    {
      times(before);
      copy_lib(argv[2], argv[3], (int) strtol(argv[4], NULL ,10), (int) strtol(argv[5], NULL, 10));
      times(after);
      printTime(before, after);
    }
    else if(strcmp(argv[6], "lib") == 0)
    {
      times(before);
      copy_sys(argv[2], argv[3], (int) strtol(argv[4], NULL ,10), (int) strtol(argv[5], NULL, 10));
      times(after);
      printTime(before, after);
    }
    else
    {
      printf("Invalid argument\n");
      return 1;
    }
    free(before);
    free(after);
    return 0;
  }
  printf("Too many arguments\n");
  return 1;
}
