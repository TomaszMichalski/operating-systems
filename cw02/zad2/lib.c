#define _XOPEN_SOURCE 500
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <ftw.h>
#include "lib.h"

int dateCompare(time_t first, time_t second)
{
  struct tm* a = (struct tm*) malloc(sizeof(struct tm));
  struct tm* b = (struct tm*) malloc(sizeof(struct tm));
  a = localtime_r(&first, a);
  b = localtime_r(&second, b);
  int out = a->tm_year - b->tm_year == 0
    ? (a->tm_mon - b->tm_mon == 0
      ? (a->tm_mday - b->tm_mday == 0
        ? (a->tm_hour - b->tm_hour == 0
          ? (a->tm_min - b->tm_min == 0
            ? 0
            : a->tm_min - b->tm_min)
          : a->tm_hour - b->tm_hour)
        : a->tm_mday - b->tm_mday)
      : a->tm_mon - b->tm_mon)
    : a->tm_year - b->tm_year;
  free(a);
  free(b);
  return out;
}

void printDirectoryTree(char *dirname, char *op, time_t userTime)
{
  DIR* dir = opendir(dirname);
  if(dir == NULL)
  {
    printf("Directory open error\n");
    return;
  }

  struct stat *buf;
  struct dirent *file;
  while((file = readdir(dir)) != NULL)
  {
    if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
    {
      continue;
    }
    char* absolutePath = calloc(1000, sizeof(char));
    realpath(dirname, absolutePath);
    strcat(absolutePath, "/");
    strcat(absolutePath, file->d_name);
    buf = (struct stat*) malloc(sizeof(struct stat));
    //char* path = (char*) malloc(strlen(dirname) * sizeof(char));
    //char* filepathBuf = (char*) malloc(PATH_MAX*sizeof(char));
    //strcpy(path, dirname);
    //strcat(path, "/");
    //strcat(path, file->d_name);

    //printf("========%s\n",absolutePath);
    stat(absolutePath, buf);
    if(strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 && S_ISDIR(buf->st_mode))
    {
      printDirectoryTree(absolutePath, op, userTime);
    }
    int printable = 0;
    int dateComp = dateCompare(buf->st_mtime, userTime);
    if(strcmp(op, "=") == 0 && dateComp == 0)
      printable = 1;
    if(strcmp(op, "<") == 0 && dateComp < 0)
      printable = 1;
    if(strcmp(op, ">") == 0 && dateComp > 0)
      printable = 1;
    if(printable && !S_ISLNK(buf->st_mode) && S_ISREG(buf->st_mode))
    {
      char* filepathBuf = (char*) malloc(PATH_MAX*sizeof(char));
      printf("%s\t\t%d\t", absolutePath, (int)buf->st_size);
      printf(buf->st_mode & S_IRUSR ? "r" : "-");
      printf(buf->st_mode & S_IWUSR ? "w" : "-");
      printf(buf->st_mode & S_IXUSR ? "x" : "-");
      printf(buf->st_mode & S_IRGRP ? "r" : "-");
      printf(buf->st_mode & S_IWGRP ? "w" : "-");
      printf(buf->st_mode & S_IXGRP ? "x" : "-");
      printf(buf->st_mode & S_IROTH ? "r" : "-");
      printf(buf->st_mode & S_IWOTH ? "w" : "-");
      printf(buf->st_mode & S_IXOTH ? "x" : "-");
      char* parsedTime = (char*) malloc(sizeof(char) * 18);
      time_t mtime = buf->st_mtime;
      strftime(parsedTime, 18, "%Y %b %d %H:%M", localtime(&mtime));
      printf("\t%s:00\n", parsedTime);
      free(parsedTime);
    }
    free(buf);
    free(absolutePath);
  }
  closedir(dir);
}

char* operator;
time_t uTime;

int nftwfunc(const char* filepath, const struct stat *buf, int fileflags, struct FTW *ftw)
{
  int printable = 0;
  int dateComp = dateCompare(buf->st_mtime, uTime);
  if(strcmp(operator, "=") == 0 && dateComp == 0)
    printable = 1;
  if(strcmp(operator, "<") == 0 && dateComp < 0)
    printable = 1;
  if(strcmp(operator, ">") == 0 && dateComp > 0)
    printable = 1;
  if(printable  && S_ISREG(buf->st_mode))
  {
    char* filepathBuf = (char*) malloc(PATH_MAX*sizeof(char));
    printf("%s\t\t%d\t", realpath(filepath,filepathBuf), (int)buf->st_size);
    printf(buf->st_mode & S_IRUSR ? "r" : "-");
    printf(buf->st_mode & S_IWUSR ? "w" : "-");
    printf(buf->st_mode & S_IXUSR ? "x" : "-");
    printf(buf->st_mode & S_IRGRP ? "r" : "-");
    printf(buf->st_mode & S_IWGRP ? "w" : "-");
    printf(buf->st_mode & S_IXGRP ? "x" : "-");
    printf(buf->st_mode & S_IROTH ? "r" : "-");
    printf(buf->st_mode & S_IWOTH ? "w" : "-");
    printf(buf->st_mode & S_IXOTH ? "x" : "-");
    char* parsedTime = (char*) malloc(sizeof(char) * 18);
    time_t mtime = buf->st_mtime;
    strftime(parsedTime, 18, "%Y %b %d %H:%M", localtime(&mtime));
    printf("\t%s:00\n", parsedTime);
    free(filepathBuf);
  }
  return 0;
}

void printDirectoryTreeNftw(char *dirname, char *op, time_t userTime)
{
  operator = op;
  uTime = userTime;
  nftw(dirname, nftwfunc, 100, FTW_PHYS);
}
