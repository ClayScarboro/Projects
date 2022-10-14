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
   FILE *fp;			// File pointer.
   char *trace_file;		// This variable holds the trace file name.
   cache_params_t params;	// Look at the sim.h header file for the definition of struct cache_params_t.
   char rw;			// This variable holds the request's type (read or write) obtained from the trace.
   uint32_t addr;		// This variable holds the request's address obtained from the trace.
				// The header file <inttypes.h> above defines signed and unsigned integers of various sizes in a machine-agnostic way.  "uint32_t" is an unsigned integer of 32 bits.

   // Exit with an error if the number of command-line arguments is incorrect.
   if (argc != 9) {
      printf("Error: Expected 8 command-line arguments but was provided %d.\n", (argc - 1));
      exit(EXIT_FAILURE);
   }
    
   // "atoi()" (included by <stdlib.h>) converts a string (char *) to an integer (int).
   params.BLOCKSIZE = (uint32_t) atoi(argv[1]);
   params.L1_SIZE   = (uint32_t) atoi(argv[2]);
   params.L1_ASSOC  = (uint32_t) atoi(argv[3]);
   params.L2_SIZE   = (uint32_t) atoi(argv[4]);
   params.L2_ASSOC  = (uint32_t) atoi(argv[5]);
   params.PREF_N    = (uint32_t) atoi(argv[6]);
   params.PREF_M    = (uint32_t) atoi(argv[7]);
   trace_file       = argv[8];

   // Open the trace file for reading.
   fp = fopen(trace_file, "r");
   if (fp == (FILE *) NULL) {
      // Exit with an error if file open failed.
      printf("Error: Unable to open file %s\n", trace_file);
      exit(EXIT_FAILURE);
   }
    
   // Print simulator configuration.
   printf("===== Simulator configuration =====\n");
   printf("BLOCKSIZE:  %u\n", params.BLOCKSIZE);
   printf("L1_SIZE:    %u\n", params.L1_SIZE);
   printf("L1_ASSOC:   %u\n", params.L1_ASSOC);
   printf("L2_SIZE:    %u\n", params.L2_SIZE);
   printf("L2_ASSOC:   %u\n", params.L2_ASSOC);
   printf("PREF_N:     %u\n", params.PREF_N);
   printf("PREF_M:     %u\n", params.PREF_M);
   printf("trace_file: %s\n", trace_file);
   printf("===================================\n");
	
	cacheInstance L1(params.BLOCKSIZE,params.L1_SIZE,params.L1_ASSOC);
	cacheInstance L2(params.BLOCKSIZE,params.L2_SIZE,params.L2_ASSOC);

   // Read requests from the trace file and echo them back.
   while (fscanf(fp, "%c %x\n", &rw, &addr) == 2) {	// Stay in the loop if fscanf() successfully parsed two tokens as specified.
      if (rw == 'r')
         printf("r %x\n", addr);
      else if (rw == 'w')
         printf("w %x\n", addr);
      else {
         printf("Error: Unknown request type %c.\n", rw);
	 exit(EXIT_FAILURE);
      }

      ///////////////////////////////////////////////////////
      // Issue the request to the L1 cache instance here.
      ///////////////////////////////////////////////////////
    }

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
    int debugAssoc = 0;
    int debugSets = 0;
	
    for(int i = 0; i < numSets; ++i){
	vector<int> x;
	for(int k = 0; k < assoc; k++){
	     x.push_back(0);
		++debugAssoc;
	}
	cacheStorage.push_back(x);
	    ++debugSets;
    }
	
    printf("made chace with %d sets %d total memblocks",debugSets,debugAssoc);	

}

memBlock::memBlock(){
    // initiating empty memoryBlock
    validBit = 0;
    dirtyBit = 0;
    value = 0;
    lruVal = 9999;
}

