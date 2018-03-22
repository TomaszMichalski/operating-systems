
struct BlockArray
{
    char** blockArray; //actual pointer
    int N; //size
    int BLOCK_SIZE;
};

struct BlockArrayStatic
{
    char staticArray[100][100]; //static 2d array
};

struct BlockArray *createArray(int N, int BLOCK_SIZE);
struct BlockArrayStatic *createArrayStatic();
void deleteArray(struct BlockArray *array);
void deleteArrayStatic(struct BlockArrayStatic *array);
char *createBlock(struct BlockArray *array, int index, int size);
char *createBlockWithValue(struct BlockArray *array, int index, char *value);
void setBlockValue(struct BlockArray *array, int index, char* value);
void setBlockValueStatic(struct BlockArrayStatic *array, int index, char *value);
void deleteBlock(struct BlockArray *array, int index);
void deleteBlocks(struct BlockArray *array, int startingIndex, int blocks);
void deleteThenCreateBlock(struct BlockArray *array, int index, int size);
void deleteThenCreateBlocks(struct BlockArray *array, int startIndex, int blocks, int size);
void eraseBlockStatic(struct BlockArrayStatic *array, int index);
void eraseBlocksStatic(struct BlockArrayStatic *array, int startingIndex, int blocks);
char *getBlockWithClosestAsciiValue(struct BlockArray *array, int k);
char *getBlockWithClosestAsciiValueStatic(struct BlockArrayStatic *array, int k);
int getAsciiCodeSum(char* block);