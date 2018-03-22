#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "lib.h"

char* generateRandomString(int size)
{
  char* result = (char*) malloc(size*sizeof(char));
  char* base = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";
  for(int i = 0; i < size - 1; i++)
  {
    result[i] = base[rand() % strlen(base)];
  }
  result[size - 1] = 10;
  return result;
}

void generate(char *filename, int records, int size)
{
  int desc = open(filename, O_WRONLY | O_CREAT, S_IRWXU); //open file, if it does not exist - create
  if(desc == -1)
  {
    printf("File open error\n");
    return;
  }
  for(int i = 0; i < records; i++)
  {
    write(desc, generateRandomString(size), size); //write randomly generated string
  }
  close(desc); //close file
}

void copy_sys(char *src, char *dest, int records, int buf_size)
{
  int src_d = open(src, O_RDONLY);
  if(src_d == -1)
  {
    printf("File open error\n");
    return;
  }
  int dest_d = open(dest, O_WRONLY | O_CREAT, S_IRWXU); //open file, if it does not exist - create
  if(dest_d == -1)
  {
    printf("File open error\n");
    return;
  }
  char *buffer = (char*) malloc(buf_size * sizeof(char));
  int charsRead;
  for(int i = 0; i < records; i++)
  {
    charsRead = read(src_d, buffer, buf_size);
    if(charsRead > 0)
      write(dest_d, buffer, charsRead);
    else return;
  }
  close(src_d);
  close(dest_d);
  free(buffer);
}

void sort_sys(char* filename, int records, int buf_size)
{
  int file = open(filename, O_RDWR);
  if(file == -1)
  {
    printf("File open error\n");
    return;
  }
  char *regA = (char*) malloc(buf_size * sizeof(char));
  char *regB = (char*) malloc(buf_size * sizeof(char));

  for(int i = 0; i < records; i++)
  {
    lseek(file, i * buf_size, SEEK_SET);
    read(file, regA, buf_size);
    for(int j = 0; j < i; j++)
    {
      lseek(file, j * buf_size, SEEK_SET);
      read(file, regB, buf_size);
      if(regB[0] > regA[0])
      {
        lseek(file, j * buf_size, SEEK_SET);
        write(file, regA, buf_size);
        lseek(file, i * buf_size, SEEK_SET);
        write(file, regB, buf_size);
        char *swap = regA;
        regA = regB;
        regB = swap;
      }
    }
  }

  close(file);
  free(regA);
  free(regB);
}

void copy_lib(char *src, char *dest, int records, int buf_size)
{
  FILE *fsrc = fopen(src, "r");
  if(fsrc == NULL)
  {
    printf("File open error\n");
    return;
  }
  FILE *fdest = fopen(dest, "w");
  if(fsrc == NULL)
  {
    printf("File open error\n");
    return;
  }
  int charsRead;
  char* buffer = (char*) malloc(buf_size * sizeof(char));
  for(int i = 0; i < records; i++)
  {
    charsRead = fread(buffer, sizeof(char), buf_size, fsrc);
    if(charsRead > 0)
      fwrite(buffer, sizeof(char), charsRead, fdest);
    else return;
  }
  fclose(fsrc);
  fclose(fdest);
  free(buffer);
}

void sort_lib(char* filename, int records, int buf_size)
{
  FILE* file = fopen(filename, "r+");
  if(file == NULL)
  {
    printf("File open error\n");
    return;
  }
  char *regA = (char*) malloc(buf_size * sizeof(char));
  char *regB = (char*) malloc(buf_size * sizeof(char));

  for(int i = 0; i < records; i++)
  {
    fseek(file, i * buf_size, 0);
    fread(regA, sizeof(char), buf_size, file);
    for(int j = 0; j < i; j++)
    {
      fseek(file, j * buf_size, 0);
      fread(regB, sizeof(char), buf_size, file);
      if(regB[0] > regA[0])
      {
        fseek(file, j * buf_size, 0);
        fwrite(regA, sizeof(char), buf_size, file);
        fseek(file, i * buf_size, 0);
        fwrite(regB, sizeof(char), buf_size, file);
        char *swap = regA;
        regA = regB;
        regB = swap;
      }
    }
  }

  fclose(file);
  free(regA);
  free(regB);
}
