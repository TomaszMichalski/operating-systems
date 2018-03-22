#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "library.h"

/*
 * Returns pointer to struct containing the dynamic array
 * @N - size of array of pointers
 */
struct BlockArray *createArray(int N, int BLOCK_SIZE)
{
    if(N < 0) //error control
        return NULL;

    struct BlockArray *blockArray = malloc(sizeof(struct BlockArray));
    char** array = (char**)calloc((size_t)N, sizeof(char*));
    blockArray->blockArray = array;
    blockArray->N = N;
    blockArray->BLOCK_SIZE = BLOCK_SIZE;

    return blockArray;
}

/*
 * STATIC VERSION
 * Returns pointer to struct containing the static array
 */
struct BlockArrayStatic *createArrayStatic()
{
    struct BlockArrayStatic *blockArray = malloc(sizeof(struct BlockArrayStatic));
    for(int i = 0; i < 100; i++)
        blockArray->staticArray[i][0] = '\0';

    return blockArray;
}

/*
 * Fully frees memory of array of pointers
 * @array - pointer to BlockArray struct
 */
void deleteArray(struct BlockArray *array)
{
    if(array == NULL) //error control
        return;
    if(array->blockArray != NULL) //more error control
    {
        for(int i = 0; i < array->N; i++)
        {
            if(array->blockArray[i] != NULL) //error control
            {
                free(array->blockArray[i]);
                array->blockArray[i] = NULL;
            }
        }
        free(array->blockArray);
        array->blockArray = NULL;
    }
    free(array);
}

/*
 * STATIC VERSION
 * Frees memory taken by struct containing static array
 * @array - pointer to BlockArrayStatic struct
 */
void deleteArrayStatic(struct BlockArrayStatic *array)
{
    if(array == NULL)
        return;
    free(array);
}

/*
 * Creates block of the array with specified size
 * @array - pointer to BlockArray struct
 * @index - index of block
 * @size - size of block
 */
char *createBlock(struct BlockArray *array, int index, int size)
{
    if(array == NULL || index >= array->N || index < 0) //error control
        return NULL;
    array->blockArray[index] = calloc((size_t)size, sizeof(char));
    return array->blockArray[index];
}

/*
 * Creates block of the array and assigns it with specified value
 * @array - pointer to BlockArray struct
 * @index - index of block
 * @value - value which will be assigned to the block
 */
char *createBlockWithValue(struct BlockArray *array, int index, char *value)
{
    if(array == NULL || index >= array->N || index < 0) //error control
        return NULL;
    array->blockArray[index] = calloc(strlen(value), sizeof(char));
    strncpy(array->blockArray[index], value, (size_t)array->BLOCK_SIZE);
    return array->blockArray[index];
}

/*
 * Assigns given value to specified block
 * @array - pointer to BlockArray struct
 * @index - index of block which value will be set
 * @value - new value of the block
 */
void setBlockValue(struct BlockArray *array, int index, char* value)
{
    if(array == NULL || array->blockArray[index] == NULL)
        return;
    strncpy(array->blockArray[index], value, (size_t)array->BLOCK_SIZE);
}

/*
 * STATIC VERSION
 * Assings given value to specified block
 * @array - pointer to BlockArrayStatic struct
 * @index - index of block which value will be set
 * @value - new value of the block
 */
void setBlockValueStatic(struct BlockArrayStatic *array, int index, char *value)
{
    if(array == NULL) //error control
        return;
    strncpy(array->staticArray[index], value, 100);
}

/*
 * Deletes block, then creates it again with given size
 * @array - pointer to BlockArray struct
 * @index - block index
 * @size - size of block
 */
void deleteThenCreateBlock(struct BlockArray *array, int index, int size)
{
    if(array == NULL || index < 0 || index >= array->N || array->blockArray[index] == NULL) //error control
        return;
    deleteBlock(array, index);
    createBlock(array, index, size);
}

/*
 * Alternately deletes and creates specified number of blocks
 * in an array, starting from given starting index.
 * Blocks are created with given size
 * @array - pointer to BlockArray struct
 * @startIndex - starting index
 * @blocks - number of blocks to process
 * @size - size of created blocks
 */
void deleteThenCreateBlocks(struct BlockArray *array, int startIndex, int blocks, int size)
{
    for(int i = 0; i < blocks; i++)
    {
        deleteThenCreateBlock(array, startIndex + i, size);
    }
}

/*
 * Deletes block of specified index
 * @array - pointer to BlockArray struct
 * @index - index of block
 */
void deleteBlock(struct BlockArray *array, int index)
{
    if(array == NULL || array->blockArray[index] == NULL || index < 0 || index >= array->N) //error control
        return;
    free(array->blockArray[index]);
    array->blockArray[index] = NULL;
}

/*
 * Deletes blocks starting with specified index
 * @array - pointer to BlockArray struct
 * @startingIndex - index of first block to delete
 * @blocks - number of blocks to delete
 */
void deleteBlocks(struct BlockArray *array, int startingIndex, int blocks)
{
    for(int i = 0; i < blocks; i++)
        deleteBlock(array, startingIndex + i);
}

/*
 * STATIC VERSION
 * 'Erases' the value in specified block with setting it's first character to '\0'
 * @array - pointer to BlockArrayStatic struct
 * @index - index of block
 */
void eraseBlockStatic(struct BlockArrayStatic *array, int index)
{
    if(array == NULL)
        return;
    array->staticArray[index][0] = '\0';
}

/*
 * STATIC VERSION
 * 'Erases' the values in blocks with setting it's first characters to '\0'
 * @array - pointer to BlockArrayStatic struct
 * @startingIndex - starting index of erasure
 * @blocks - number of blocks to erase
 */
void eraseBlocksStatic(struct BlockArrayStatic *array, int startingIndex, int blocks)
{
    for(int i = 0; i < blocks; i++)
        eraseBlockStatic(array, startingIndex + i);
}

/*
 * Searches block with sum of ASCII codes closest to specified block
 * @array - pointer to BlockArray struct
 * @k - index of specified block
 */
char *getBlockWithClosestAsciiValue(struct BlockArray *array, int k)
{
    if(array == NULL || k < 0 || k >= array->N || array->blockArray[k] == NULL) //error control
        return NULL;
    int kSum = getAsciiCodeSum(array->blockArray[k]);
    int diff = INT_MAX;
    int newDiff;
    char* result = NULL;
    for(int i = 0; i < array->N; i++)
    {
        if(i != k)
        {
            newDiff = abs(kSum - getAsciiCodeSum(array->blockArray[i]));
            if(newDiff < diff)
            {
                result = array->blockArray[i];
                diff = newDiff;
            }
        }
    }
    return result;
}

/*
 * STATIC VERSION
 * Searches block index with sum of ASCII codes closest to specified block
 * @array - pointer to BlockArrayStatic struct
 * @k - index of specified block
 */
char* getBlockWithClosestAsciiValueStatic(struct BlockArrayStatic *array, int k)
{
    if(array == NULL || k < 0 || k >= 100)
        return NULL;
    int kSum = getAsciiCodeSum(array->staticArray[k]);
    int diff = INT_MAX;
    int newDiff;
    char* result = NULL;
    for(int i = 0; i < 100; i++)
    {
        if(i != k)
        {
            newDiff = abs(kSum - getAsciiCodeSum(array->staticArray[i]));
            if(newDiff < diff)
            {
                result = array->staticArray[i];
                diff = newDiff;
            }
        }
    }
    return result;
}

/*
 * Calculates ASCII sum of block
 * @block - pointer to array of characters
 */
int getAsciiCodeSum(char* block)
{
    if(block == NULL) //error control
        return 0;
    char* tmp = block;
    int sum = 0;
    while(*tmp != '\0')
    {
        sum += (int)(*tmp);
        tmp++;
    }
    return sum;
}
