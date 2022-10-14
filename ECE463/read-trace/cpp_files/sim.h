#ifndef SIM_CACHE_H
#define SIM_CACHE_H

using namespace std;

typedef 
struct {
   uint32_t BLOCKSIZE;
   uint32_t L1_SIZE;
   uint32_t L1_ASSOC;
   uint32_t L2_SIZE;
   uint32_t L2_ASSOC;
   uint32_t PREF_N;
   uint32_t PREF_M;
} cache_params_t;

class memBlock{
public:
    int value;
    int dirtyBit, validBit, lruVal;
    memBlock();
};

class cacheInstance {
public:
    int numBlocks, numSets, indexBits, blockOffsetBits, tagBits;
    vector< vector<memBlock> > cacheStorage;

    cacheInstance(int blockSize, int size, int assoc);

    ~cacheInstance()= default;;

    int checkCache(uint32_t addr, int assoc);

    void editCache(memBlock addr);

};

void checkCache(cacheInstance Lx, uint32_t addr);

#endif
