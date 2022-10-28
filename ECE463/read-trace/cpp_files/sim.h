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
    uint32_t tag;
    int dirtyBit, validBit, lruVal;
    memBlock();
};

class cacheInstance {
public:
    int numBlocks, numSets, indexBits, blockOffsetBits, tagBits, assoc;
    vector< vector<memBlock> > cacheStorage;

    cacheInstance(int blockSize, int size, int assocSet);

    ~cacheInstance()= default;;

    int checkCache(uint32_t addr);

    int editCache(uint32_t addr, int isDirty);
   
   void writeBack(int addr);

};

void checkCache(cacheInstance Lx, uint32_t addr);

#endif
