#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ftw.h>
#include "lib.h"

//char *strptime(char *a, char *b, struct tm *c);

int main(int argc, char **argv)
{
  if(argc < 4)
  {
    printf("Too few arguments\n");
    return 1;
  }
  if(argc > 4)
  {
    printf("Too many arguments\n");
    return 1;
  }

  struct tm *timeParsed = (struct tm*) malloc(sizeof(struct tm));
  strptime(strcat(argv[3], ":00"), "%Y %b %d %H:%M:%S", timeParsed);
  time_t timeParsedInSec = mktime(timeParsed);

  printDirectoryTree(argv[1], argv[2], timeParsedInSec);
  printf("========\n");
  printDirectoryTreeNftw(argv[1], argv[2], timeParsedInSec);
  free(timeParsed);
  return 0;
}
