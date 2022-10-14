#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <cmath>
#include <vector>
#include "sim.h"

using namespace std;

/*  "argc" holds the number of command-line arguments.
    "argv[]" holds the arguments themselves.

    Example:
    ./sim 32 8192 4 262144 8 3 10 gcc_trace.txt
    argc = 9
    argv[0] = "./sim"
    argv[1] = "32"
    argv[2] = "8192"
    ... and so on
*/
int main (int argc, char *argv[]) {

    cacheInstance L1(32,256,2);

    printf("WORKING! lets see\n");

    return(0);
}

//Returns 0 if ADDR absent from cache, returns iterator + 1 if ADDR present in cache.
int cacheInstance::checkCache(uint32_t addr, int assoc){

    //get index value
    int indexVal = addr >> this->blockOffsetBits;
    int numIndexBits = pow(2,this->indexBits) - 1;
    indexVal = indexVal | numIndexBits;

    //get tag value
    int tagVal = addr >> (this->indexBits + this->blockOffsetBits);

    //compare tag value @ index
    for( int i = 0; i < assoc; i++){
        if(1 == tagVal){
            return i + 1;
        }
    }

    return 0;

}

void cacheInstance::editCache(int value,memBlock target){
    if(value == target.value){
        // no need to write anything
        return;
    }

    target.value = value;
    target.dirtyBit = 1;

}

cacheInstance::cacheInstance(int blockSize, int size, int assoc){

    //getting parameters-
    numBlocks = size / blockSize;
    numSets = size / (assoc * blockSize);
    indexBits = log2(numSets);
    blockOffsetBits = log2(blockSize);
    tagBits = blockSize - indexBits - blockOffsetBits;


}

memBlock::memBlock(){
    // initiating empty memoryBlock
    validBit = 0;
    dirtyBit = 0;
    value = 0;
    lruVal = 9999;
}

