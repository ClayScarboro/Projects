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
  
	
   	int outa = 0;
	int outb = 0;
	int outc = 0;
	int outd = 0;
	float oute = 0;
	int outf = 0;
	int outg = 0;
	int outh = 0;
	int outi = 0;
	int outj = 0;
	int outk = 0;
	int outl = 0;
	int outm = 0;
	float outn = 0;
	int outo = 0;
	int outp = 0;
	int outq = 0;
	

// Read requests from the trace file and echo them back.
	while (fscanf(fp, "%c %x\n", &rw, &addr) == 2) {	// Stay in the loop if fscanf() successfully parsed two tokens as specified.
		if (rw == 'r'){
			printf("r %x\n", addr);
		}
		else if (rw == 'w'){
			printf("w %x\n", addr);
		}
		else {
			printf("Error: Unknown request type %c.\n", rw);
		exit(EXIT_FAILURE);
		}
	//Cache Time
	uint32_t dirtyAddr;
		
//---------------------------------------------------- HAS L2 CACHE ------------------------------------------------------------------------ 
		
		if(params.L2_SIZE > 0){
			if(rw == 'r'){
				//Read Request
				++outa;
				if(!L1.checkCache(addr)){
					//Not in L1, check L2
					++outb;
					++outh;
					if(!L2.checkCache(addr)){
						++outq;
						//Miss, bring into L2 from Main Mem and then L1 from L2
						++outi;
						dirtyAddr = L2.editCache(addr,0);
						if( 0 > dirtyAddr){
							//Must L2 Writeback to MM
							++outo;
							++outq;
							L2.editCache(dirtyAddr,1);
						}
						
						dirtyAddr = L1.editCache(addr,0);
						if( 0 > dirtyAddr){
							//Must L2 Writeback to MM
							++outo;
							++outq;
							L1.editCache(dirtyAddr,1);
						}
					}
					else {
						//Found in L2, Bring into L1
						dirtyAddr = L1.editCache(addr,0);
						if( 0 > dirtyAddr){
							//Must L2 Writeback to MM
							++outo;
							++outq;
							L2.editCache(dirtyAddr,1);
						}
					}
				} 
				else {
					//Hit, read successful
				}
			} 
			else {
				//Write Request
				++outc;
				if(!L1.checkCache(addr)){
					++outd;
					//Not in L1, check L2 to pull in
					++outh;
					if(!L2.checkCache(addr)){
						++outm;
						++outq;
						//Not in L2, bring into MM
						dirtyAddr = L2.editCache(addr,0);
						if( 0 > dirtyAddr){
							//Must L2 Writeback to MM
							++outo;
							++outq;
						}
						
						//Now in L2, Write to L1
				
						dirtyAddr = L1.editCache(addr,1);
						if( 0 > dirtyAddr){
							//Must L2 Writeback to MM
							++outo;
							++outq;
							L2.editCache(dirtyAddr,1);
						}
					}
					else {
						//Found in L2, Bring into L1
						dirtyAddr = L1.editCache(addr,1);
						if( 0 > dirtyAddr){
							//Must L2 Writeback to MM
							++outo;
							++outq;
							L2.editCache(dirtyAddr,1);
						}
					}
				} 
				else {
					//Hit, Writing
					dirtyAddr = L1.editCache(addr,1);
						if( 0 > dirtyAddr){
							//Must L2 Writeback to MM
							++outo;
							++outq;
							L2.editCache(dirtyAddr,1);
					}
				}
			}
			
//---------------------------------------------------- NO L2 CACHE ------------------------------------------------------------------------ 
			
		} else { 
			if(rw == 'r'){
				//Read Request
				++outa;
				if(!L1.checkCache(addr)){
					//Not in L1, move from mem
					++outb;
					++outh;
					dirtyAddr = L1.editCache(addr,0);
						if( 0 > dirtyAddr){
							//Must L1 Writeback to MM
							++outf;
							++outq;
					}
				} 
				else {
					//Hit, read successful
				}
			} 
			else {
				//Write Request
				++outc;
				if(!L1.checkCache(addr)){
					//not in L1, bring in form mem
					++outd;
					dirtyAddr = L1.editCache(addr,0);
						if( 0 > dirtyAddr){
							//Must L1 Writeback to MM
							++outf;
							++outq;
					}
				} 
				else {
					//Hit, Writing
					dirtyAddr = L1.editCache(addr,1);
						if( 0 > dirtyAddr){
							//Must L1 Writeback to MM
							++outf;
							++outq;
					}
				}
			}
		}
	} 

	//DONE SIMULATING CHACE! COLLECT OUTPUTS!
	
	oute = outb + outd / (outa + outc);
	outn = outi / outh;
	
	//PRINTING STATS
	printf("===== L1 contents =====\n");
	for(int i = 0; i < L1.numSets; i++){
		printf("set	%d:", i);
		for(int k = 0; k < L1.assoc; k++){
			printf("	%x:", L1.cacheStorage[i][k].tag);
			if(L1.cacheStorage[i][k].dirtyBit == 1){
				printf(" D");	
			}
			else printf("	");
		}
		printf("\n");
	}
	
	if(params.L2_SIZE > 0){
		printf("===== L2 contents =====\n");
		for(int i = 0; i < L2.numSets; i++){
			printf("set	%d:", i);
			for(int k = 0; k < L2.assoc; k++){
				printf("	%x:", L2.cacheStorage[i][k].tag);
				if(L2.cacheStorage[i][k].dirtyBit == 1){
					printf(" D");	
				}
				else printf("  ");
			}
			printf("\n");
		}
	}
	
	printf("===== Measurements =====\n");
  	printf("a. L1 reads:  %u\n", outa);
	printf("b. L1 read misses:  %u\n", outb);
	printf("c. L1 writes:  %u\n", outc);
	printf("d. L1 write misses:  %u\n", outd);
	printf("e. L1 miss rate:  %f\n", oute);
	printf("f. L1 writebacks:  %u\n", outf);
	printf("g. L1 prefetches:  %u\n", outg);
	printf("h. L1 reads (demand):  %u\n", outh);
	printf("i. L1 reads misses (demand):  %u\n", outi);
	printf("j. L1 reads (prefetch):  %u\n", outj);
	printf("k. L1 read misses (prefetch):  %u\n", outk);
	printf("l. L2 writes:  %u\n", outl);
	printf("m. L2 write misses:  %u\n", outm);
	printf("n. L2 miss rate:  %f\n", outn);
	printf("o. L2 writebacks:  %u\n", outo);
	printf("p. L2 prefetches:  %u\n", outp);
	printf("q. memory traffic:  %u\n", outq);
	printf("===================================\n");
	
	return(0);
}

//Returns 0 if ADDR absent from cache, returns iterator + 1 if ADDR present in cache.
int cacheInstance::checkCache(uint32_t addr){

    //get index value
    int indexVal = addr >> this->blockOffsetBits;
    int numIndexBits = pow(2,this->indexBits) - 1;
    indexVal = indexVal & numIndexBits;

    //get tag value
    uint32_t tagVal = addr >> (this->indexBits + this->blockOffsetBits);

    //compare tag value @ index
    for(int i = 0; i < this->assoc; ++i){
	if(this->cacheStorage[indexVal][i].validBit == 0) continue;
	if(this->cacheStorage[indexVal][i].tag == tagVal){
		printf("HIT! %x @ index %d way %d\n",tagVal,indexVal,i);
		
		//Increment all LRU by 1
		for(int b = 0; b < this->assoc; b++){
			if(this->cacheStorage[indexVal][b].dirtyBit == 1){
				this->cacheStorage[indexVal][b].lruVal++;	
			}
		}
		
		return i + 1;
	}
    }
    printf("MISS! %x not in cache\n",tagVal);
    return 0;

}

int cacheInstance::editCache(uint32_t addr, int isDirty){
    //get index value
	
    int doWriteBack = 0;
	
    int indexVal = addr >> this->blockOffsetBits;
    int numIndexBits = pow(2,this->indexBits) - 1;
    indexVal = indexVal & numIndexBits;
    printf("Index Val: %d\n",indexVal); 
	
    //get tag value
    uint32_t tagVal = addr >> (this->indexBits + this->blockOffsetBits);
    
	
	
	//finding open slot
	int LRUIndex;
	int LRUHighest = 0;
	for(int i = 0; i < this->assoc; i++){
		
		//Going untill we find empty memblock
		if( this->cacheStorage[indexVal][i].validBit == 1){ 
			continue;
		}
		
		//found empty, place and increment ALL LRU by 1
		else{
			
			if(this->cacheStorage[indexVal][i].dirtyBit == 1){
				//MUST WRITEBACK!	
				doWriteBack = cacheStorage[indexVal][i].tag;	
			}
			
			//Setting New Info
			
			this->cacheStorage[indexVal][i].validBit = 1;
			this->cacheStorage[indexVal][i].lruVal = 0;
			this->cacheStorage[indexVal][i].tag = tagVal;
			if(isDirty){ 	
				this->cacheStorage[indexVal][i].dirtyBit = 1; 
			}
			else{ 
				this->cacheStorage[indexVal][i].dirtyBit = 0; 
			}
			
			//Increment all LRU by 1
		for(int b = 0; b < this->assoc; b++){
			if(this->cacheStorage[indexVal][b].dirtyBit == 1){
				this->cacheStorage[indexVal][b].lruVal++;	
			}
		}
			
			printf("Placing tag %x in set %d assoc %d\n",tagVal,indexVal,i);
		
			return doWriteBack;
		}
		if (this->cacheStorage[indexVal][i].lruVal > LRUHighest){
			LRUIndex = i;
			LRUHighest = this->cacheStorage[indexVal][i].lruVal;
		}
	}
	
	//No open slot, must evict and handle LRU
	if(this->cacheStorage[indexVal][LRUIndex].dirtyBit == 1){
	//MUST WRITEBACK!	
	doWriteBack = cacheStorage[indexVal][LRUIndex].tag;	
	}

	//Setting New Info

	this->cacheStorage[indexVal][LRUIndex].validBit = 1;
	this->cacheStorage[indexVal][LRUIndex].lruVal = 0;
	this->cacheStorage[indexVal][LRUIndex].tag = tagVal;
	if(isDirty){ this->cacheStorage[indexVal][LRUIndex].dirtyBit = 1; }
	else{ this->cacheStorage[indexVal][LRUIndex].dirtyBit = 0; }
	
	//Increment all LRU by 1
		for(int b = 0; b < this->assoc; b++){
			if(this->cacheStorage[indexVal][b].dirtyBit == 1){
				this->cacheStorage[indexVal][b].lruVal++;	
			}
		}
	
	printf("Evicting set %d assoc %d and adding tag %x\n",indexVal,LRUIndex,tagVal);
	
	return doWriteBack;

}

cacheInstance::cacheInstance(int blockSize, int size, int assocSet){

    //getting parameters-
    if(size != 0){
	    assoc = assocSet;
	    numBlocks = size / blockSize;
	    numSets = size / (assoc * blockSize);
	    indexBits = log2(numSets);
	    blockOffsetBits = log2(blockSize);
	    tagBits = blockSize - indexBits - blockOffsetBits;
	    int debugAssoc = 0;
	    int debugSets = 0;

	    for(int i = 0; i < numSets; ++i){
		vector<memBlock> setOfMem;
		for(int k = 0; k < this->assoc; k++){
		     memBlock toAdd;
		     setOfMem.push_back(toAdd);
			++debugAssoc;
		}
		cacheStorage.push_back(setOfMem);
		    ++debugSets;
	    }

	    printf("made chace with %d sets %d assoc\n",debugSets,debugAssoc);	
    }	
}

memBlock::memBlock(){
    // initiating empty memoryBlock
    validBit = 0;
    dirtyBit = 0;
    tag = -1;
    lruVal = -1;
}

