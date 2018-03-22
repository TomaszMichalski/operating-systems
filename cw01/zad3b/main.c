#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include "library.h"

char* getRandomString(int length)
{
    char *characters ="abcdefghijklmnoprstuvwxyz";
    size_t charactersLength = strlen(characters);
    char *random = (char*)malloc(length*sizeof(char));
    for(int i = 0; i < length; i++)
        random[i] = characters[rand() % charactersLength];

    return random;
}

double timeDiff(clock_t start, clock_t end){
    return (double)(end -  start) / sysconf(_SC_CLK_TCK);
}

void printTime(clock_t rStartTime, struct tms tmsStartTime, clock_t rEndTime, struct tms tmsEndTime){
  printf("delta_real:%.100lf\n", timeDiff(rStartTime, rEndTime));
  printf("delta_user:%.100f\n", timeDiff(tmsStartTime.tms_utime, tmsEndTime.tms_utime));
  printf("delta_system:%.100f\n", timeDiff(tmsStartTime.tms_stime, tmsEndTime.tms_stime));
}

int main(int argc, char *argv[])
{
    clock_t rTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    struct tms tmsTime[8];
    int currentTime = 0;

    srand((unsigned int) time(NULL));
    if (argc < 4) //too little arguments
    {
        printf("Arguments should be:\n1.number of blocks\n2.block size\n3.memory allocation mode\n4+.list of operations.");
        return 1; //error
    }
    //parsing the arguments
    printf("Parse begin\n");
    int BLOCKS = (int) strtol(argv[1], NULL, 10);
    printf("Block number parsed\n");
    int BLOCK_SIZE = (int) strtol(argv[2], NULL, 10);
    printf("Block size parsed\n");
    struct BlockArray *arrayDynamic = NULL;
    struct BlockArrayStatic *arrayStatic = NULL;
    int dynamic;
    if (strcmp(argv[3], "dynamic") == 0)
        dynamic = 1;
    else if(strcmp(argv[3], "static") == 0)
        dynamic = 0;
    else
    {
        printf("Error parsing memory allocation mode argument\n");
        return 2; //error
    }
    printf("Allocation mode parsed\n");

    rTime[currentTime] = clock();
    times(&tmsTime[currentTime]);
    currentTime++;
    //parsing operations starting from 4th argument
    for(int i = 4; i < argc; i++)
    {
        rTime[currentTime] = clock();
        times(&tmsTime[currentTime]);
	if(tmsTime[currentTime].tms_utime == (clock_t)-1)
		printf("ERROR============================\n");
        currentTime++;
        printf("Parsing arguments\n");
        if(strcmp(argv[i], "create") == 0) //create array
        {
            printf("Attempting to create array\n");
            if(dynamic)
                arrayDynamic = createArray(BLOCKS, BLOCK_SIZE);
            else
                arrayStatic = createArrayStatic();
            printf("Array created\n");
        }
        else if(strcmp(argv[i], "find") == 0) //find closest ASCII-sum element
        {
            printf("Attempting to find closest ASCII-sum element\n");
            char* closest = NULL;
            if(i + 1 >= argc)
            {
                printf("Error parsing argument: closes has no paramater\n");
                return 3; //error
            }
            int param = (int) strtol(argv[i+1], NULL, 10);
            i++;
            if(dynamic)
                closest = getBlockWithClosestAsciiValue(arrayDynamic, param);
            else
                closest = getBlockWithClosestAsciiValueStatic(arrayStatic, param);
            printf("Found: %s\n", closest);
        }
        else if(strcmp(argv[i], "replace") == 0) //delete then create blocks
        {
            printf("Attempting to replace\n");
            if(i + 2 >= argc)
            {
                printf("Error parsing argument: replace has no paramaters\n");
                return 4; //error
            }
            int param = (int) strtol(argv[i+1], NULL, 10);
            int param2 = (int) strtol(argv[i+2], NULL, 10);
            i += 2;
            if(dynamic)
            {
                deleteBlocks(arrayDynamic, param, param2);
                printf("Blocks deleted\n");
                for(int j = 0; j < param2; j++)
                {
                    createBlockWithValue(arrayDynamic, param + j, getRandomString(BLOCK_SIZE));
                }
                printf("Blocks set\n");
            }
            else
            {
                eraseBlocksStatic(arrayStatic, param, param2);
                printf("Blocks deleted\n");
                for(int j = 0; j < param2; j++)
                {
                    setBlockValueStatic(arrayStatic, param + j, getRandomString(100));
                }
                printf("Blocks set\n");
            }
        }
        else if(strcmp(argv[i], "replace_alt") == 0) //alternately delete-create blocks
        {
            if(i + 2 >= argc)
            {
                printf("Error parsing argument: replace_alt has not parameters\n");
                return 5;
            }
            int param = (int) strtol(argv[i+1], NULL, 10);
            int param2 = (int) strtol(argv[i+1], NULL, 10);
            i += 2;
            if(dynamic)
            {
                deleteThenCreateBlocks(arrayDynamic, param, param2, BLOCK_SIZE);
                printf("Blocks deleted\n");
                for(int j = 0; j < param2; j++)
                {
                    setBlockValue(arrayDynamic, param + j, getRandomString(BLOCK_SIZE));
                }
                printf("Blocks set\n");
            }
            else
            {
                eraseBlocksStatic(arrayStatic, param, param2);
                printf("Blocks deleted\n");
                for(int j = 0; j < param2; j++)
                {
                    setBlockValueStatic(arrayStatic, param + j, getRandomString(BLOCK_SIZE));
                }
                printf("Blocks set\n");
            }
        }
        rTime[currentTime] = clock();
        times(&tmsTime[currentTime]);
        currentTime++;
        printTime(rTime[currentTime-2], tmsTime[currentTime-2], rTime[currentTime-1], tmsTime[currentTime-1]);
    }
    printf("Parsing finished\n");

    printf("Attempting to free memory\n");
    if(dynamic)
        deleteArray(arrayDynamic);
    else
        deleteArrayStatic(arrayStatic);
    printf("Memory free\n");
    printf("Exiting program\n");

    return 0;
}
