/* sidkaul
 * EECS 370, University of Michigan
 * Project 4: LC-2K Cache Simulator
 * Instructions are found in the project spec.
 */

#include <stdio.h>

#define MAX_CACHE_SIZE 256
#define MAX_BLOCK_SIZE 256

extern int mem_access(int addr, int write_flag, int write_data);

enum actionType
{
    cacheToProcessor,
    processorToCache,
    memoryToCache,
    cacheToMemory,
    cacheToNowhere
};

typedef struct blockStruct
{
    int data[MAX_BLOCK_SIZE];
    int dirty;
    int valid;
    int lruLabel;
    int set;
    int tag;
} blockStruct;

typedef struct cacheStruct
{
    blockStruct blocks[MAX_CACHE_SIZE];
    int blockSize;
    int numSets;
    int blocksPerSet;
} cacheStruct;

/* Global Cache variable */
cacheStruct cache;

void printAction(int, int, enum actionType);
void printCache();
int helper(int temp, cacheStruct help) {
    int val = help.blocksPerSet * temp;
    int ans = help.blocks[val].lruLabel;
    val = help.blocksPerSet * temp;
    int fin = val;
    int one = 1;
    int keep = one + temp;
    int keeper = help.blocksPerSet * keep;
    int x = one + val;
    while (x < keeper) {
        int hold = help.blocks[x].lruLabel;
        if (ans < hold) {
            ans = help.blocks[x].lruLabel;
            fin = x;
        }
        ++x;
    }
    return fin;
}

/*
 * Set up the cache with given command line parameters. This is 
 * called once in main(). You must implement this function.
 */
void cache_init(int blockSize, int numSets, int blocksPerSet){
    cache.blocksPerSet = blocksPerSet;
    cache.numSets = numSets;
    cache.blockSize = blockSize;
    int x = 0;
    int zero = 0;
    int neg1 = -1;
    while (x < cache.numSets) {
        for (int y = 0; y < cache.blocksPerSet; ++y) {
            int temp = (x * cache.blocksPerSet) + y;
            cache.blocks[temp].tag = neg1;
            cache.blocks[temp].lruLabel = zero;
            cache.blocks[temp].valid = zero;
            cache.blocks[temp].set = x;
            cache.blocks[temp].dirty = zero;
            
        }
        ++x;
    }
}

/*
 * Access the cache. This is the main part of the project,
 * and should call printAction as is appropriate.
 * It should only call mem_access when absolutely necessary.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads (fetch/lw) and 1 for writes (sw).
 * write_data is a word, and is only valid if write_flag is 1.
 * The return of mem_access is undefined if write_flag is 1.
 * Thus the return of cache_access is undefined if write_flag is 1.
 */
int cache_access(int addr, int write_flag, int write_data) {
    int temp = addr / cache.blockSize;
    int set = temp % cache.numSets;
    int val = addr / cache.blockSize / cache.numSets;
    int blockOffset = addr % cache.blockSize;
    int zero = 0;
    int one = 1;
    switch(write_flag) {
        case '1': {
            int help = cache.blocksPerSet * set;
            int temp1 = set + one;
            temp = cache.blocksPerSet * temp1;
            for (int x = 0; temp > help && val != cache.blocks[help].tag; ++x) {
                    ++help;
            }   
            if (temp == help || temp < help) {
                int keeper = cache.blocksPerSet * set;
                int temp3 = one + set;
                int hold8 = cache.blocksPerSet * temp3;
                for (int i = 0; hold8 > keeper && cache.blocks[keeper].valid; ++i) {
                    ++keeper;
                }
                if (hold8 > keeper || hold8 < keeper) {
                    int x = 0;
                    while (x < cache.blockSize) {
                        int hold = addr - blockOffset + x;
                        cache.blocks[keeper].data[x] = mem_access(hold,zero,write_data);
                        //printf("Mem1");
                        //printf("%d", cache.blocks[keeper].data[x]);
                        ++x;
                    }
                    printAction(addr - blockOffset,cache.blockSize,memoryToCache);
                    printf("daddy");
                    cache.blocks[keeper].tag = val;
                    cache.blocks[keeper].data[blockOffset] = write_data;
                    cache.blocks[keeper].valid = one;
                    cache.blocks[keeper].dirty = one;
                    printf("CAUGHT");
                    printAction(addr,one,processorToCache);

                    int b = cache.blocksPerSet * set;
                    int temp8 = set + one;
                    int hold7 = cache.blocksPerSet * temp8;
                    while (hold7 > b) {
                        ++cache.blocks[b].lruLabel;
                        ++b;
                    }
                    cache.blocks[keeper].lruLabel = zero;
                    return zero;
                }
                else {
                    int lru = helper(set,cache);
                    int temp0 = (cache.numSets * cache.blocks[lru].tag) + cache.blocks[lru].set;
                    int help_addr = temp0 * cache.blockSize;
                    if (cache.blocks[lru].dirty) {
                        int f = 0;
                        while (f < cache.blockSize) {
                            mem_access(f + help_addr, one, cache.blocks[lru].data[f]);
                            //printf("Mem2");
                            ++f;
                        }
                        printAction(help_addr, cache.blockSize, cacheToMemory);
                    }
                    else {
                        printAction(help_addr, cache.blockSize, cacheToNowhere);
                    }
                    int t = 0;
                    while (t < cache.blockSize) {
                        int temp5 = t + addr - blockOffset;
                        cache.blocks[lru].data[t] = mem_access(temp5, zero, write_data);
                        //printf("Mem3");
                        ++t;
                    }
                    int temp5 = addr - blockOffset;
                    printAction(temp5, cache.blockSize, memoryToCache);
                    cache.blocks[lru].dirty = one;
                    cache.blocks[lru].tag = val;
                    cache.blocks[lru].valid = one;
                    cache.blocks[lru].data[blockOffset] = write_data;
                    printf("CAUGHT");
                    printAction(addr,one,processorToCache);
                    int r = cache.blocksPerSet * set;
                    int temp6 = set + one;
                    while (r < (cache.blocksPerSet * temp6)) {
                        ++cache.blocks[r].lruLabel;
                        ++r;
                    }
                    cache.blocks[lru].lruLabel = zero;
                    return zero;
                }
            }
            else {
                cache.blocks[help].dirty = one;
                cache.blocks[help].valid = one;
                cache.blocks[help].data[blockOffset] = write_data;
                int f = cache.blocksPerSet * set;
                int temp5 = one + set;
                int holding = cache.blocksPerSet * temp5;
                while (holding > f) {
                    if ((cache.blocks[help].lruLabel < cache.blocks[f].lruLabel) || (cache.blocks[help].lruLabel == cache.blocks[f].lruLabel)) {
                    }
                    else {
                        ++cache.blocks[f].lruLabel;
                    }
                    ++f;
                }
                cache.blocks[help].lruLabel = zero;
                printf("CAUGHT");
                printAction(addr, one, processorToCache);
                return zero;
            }
            break;
        }
        default: {
            int keeper = cache.blocksPerSet * set;
            int temp11 = one + set;
            int holding = cache.blocksPerSet * temp11;
            for (int i = 0; (val != cache.blocks[keeper].tag) && keeper < holding; ++i) {
                ++keeper;
            }
            holding = cache.blocksPerSet * temp11;
            if (holding == keeper || holding < keeper) {
                int rare = cache.blocksPerSet * set;
                int temp8 = set + one;
                int hold5 = cache.blocksPerSet * temp8;
                for (int x = 0; cache.blocks[rare].valid && hold5 > rare; ++x) {
                    ++rare;
                }
                if (hold5 > rare || hold5 < rare) {
                    int f = 0;
                    while (f < cache.blockSize) {
                        int temp5 = f + addr - blockOffset;
                        cache.blocks[rare].data[f] = mem_access(temp5,zero,write_data);
                        //printf("mem4");
                        ++f;
                    }
                    cache.blocks[rare].valid = one;
                    cache.blocks[rare].dirty = zero;
                    cache.blocks[rare].tag = val;
                    int temp7 = addr - blockOffset;
                    printAction(temp7,cache.blockSize,memoryToCache);
                    int r = cache.blocksPerSet * set;
                    int hold = one + set;
                    int hold1 = cache.blocksPerSet*hold;
                    while (hold1 > r) {
                        ++cache.blocks[r].lruLabel;
                        ++r;
                    }
                    cache.blocks[rare].lruLabel = zero;

                    printAction(addr, one, cacheToProcessor);
                    int answer = cache.blocks[rare].data[blockOffset];
                    return answer;
                }
                else {
                    int lru = helper(set,cache);
                    int temp0 = (cache.numSets * cache.blocks[lru].tag) + cache.blocks[lru].set;
                    int help_addr = temp0 * cache.blockSize;
                    if (cache.blocks[lru].dirty) {
                        int f = 0;
                        while (cache.blockSize > f) {
                            mem_access(f + help_addr,one,cache.blocks[lru].data[f]);
                            //printf("Mem5");
                            ++f;
                        }
                        printAction(help_addr,cache.blockSize,cacheToMemory);
                    }
                    else {
                        printAction(help_addr,cache.blockSize,cacheToNowhere);
                    }
                    int f = 0;
                    while (cache.blockSize > f) {
                        int temp5 = f + addr - blockOffset;
                        cache.blocks[lru].data[f] = mem_access(temp5,zero,write_data);
                        //printf("Mem6");
                        ++f;
                    }
                    cache.blocks[lru].valid = one;  
                    cache.blocks[lru].tag = val;
                    cache.blocks[lru].dirty = zero;
                    int temp5 = addr - blockOffset;
                    printAction(temp5,cache.blockSize,memoryToCache);
                    
                    int r = cache.blocksPerSet * set;
                    int temper = one + set;
                    int holder = cache.blocksPerSet * temper;
                    while (holder > r) {
                        ++cache.blocks[r].lruLabel;
                        ++r;
                    }
                    cache.blocks[lru].lruLabel = zero;
                    printAction(addr, one, cacheToProcessor);
                    int final = cache.blocks[lru].data[blockOffset];
                    return final;
                }
            }
            else {
                printAction(addr, one, cacheToProcessor);
                //printf("daddy");
                int c = cache.blocksPerSet * set;
                int temp9 = one + set;
                int hold6 = cache.blocksPerSet * temp9;
                while (hold6 > c) {
                    if (cache.blocks[keeper].lruLabel > cache.blocks[c].lruLabel) {
                        ++cache.blocks[c].lruLabel;
                    }
                    ++c;
                }
                cache.blocks[keeper].lruLabel = zero;
                int final = cache.blocks[keeper].data[blockOffset];
                return final;
            }
            
        }
        //break;
    }
    return 0;
}


/*
 * print end of run statistics like in the spec. This is not required,
 * but is very helpful in debugging.
 * This should be called once a halt is reached.
 * DO NOT delete this function, or else it won't compile.
 */
void printStats(){
    printCache();
    return;
}

/*
 * Log the specifics of each cache action.
 *
 * address is the starting word address of the range of data being transferred.
 * size is the size of the range of data being transferred.
 * type specifies the source and destination of the data being transferred.
 *  -    cacheToProcessor: reading data from the cache to the processor
 *  -    processorToCache: writing data from the processor to the cache
 *  -    memoryToCache: reading data from the memory to the cache
 *  -    cacheToMemory: evicting cache data and writing it to the memory
 *  -    cacheToNowhere: evicting cache data and throwing it away
 */
void printAction(int address, int size, enum actionType type)
{
    printf("$$$ transferring word [%d-%d] ", address, address + size - 1);

    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    }
    else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    }
    else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    }
    else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    }
    else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
}

/*
 * Prints the cache based on the configurations of the struct
 * This is for debugging only and is not graded, so you may
 * modify it, but that is not recommended.
 */
void printCache()
{
    printf("\ncache:\n");
    for (int set = 0; set < cache.numSets; ++set) {
        printf("\tset %i:\n", set);
        for (int block = 0; block < cache.blocksPerSet; ++block) {
            printf("\t\t[ %i ]: {", block);
            for (int index = 0; index < cache.blockSize; ++index) {
                printf(" %i", cache.blocks[set * cache.blocksPerSet + block].data[index]);
            }
            printf(" }\n");
        }
    }
    printf("end cache\n");
}
