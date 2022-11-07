#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_bp.h"
#include <cmath>

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/
int main (int argc, char* argv[])
{
    int numPredictions = 0;
    int numMispredictions = 0;
    float predRate;
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file
    
    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.bp_name  = argv[1];
    int mode;
    // 0 = bimodal
    // 1 = gshare
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        mode = 0;
        if(argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M2       = strtoul(argv[2], NULL, 10);
        trace_file      = argv[3];
        printf("COMMAND\n%s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);
    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        mode = 1;
        if(argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M1       = strtoul(argv[2], NULL, 10);
        params.N        = strtoul(argv[3], NULL, 10);
        trace_file      = argv[4];
        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);

    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if(argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.K        = strtoul(argv[2], NULL, 10);
        params.M1       = strtoul(argv[3], NULL, 10);
        params.N        = strtoul(argv[4], NULL, 10);
        params.M2       = strtoul(argv[5], NULL, 10);
        trace_file      = argv[6];
        printf("COMMAND\n%s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);

    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }
    
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    branchPredictor mainBP(params.M2,0);
    
    char str[2];
    while(fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
       
        outcome = str[0];
        if (outcome == 't')
            printf("%lx %s\n", addr, "t");           // Print and test if file is read correctly
        else if (outcome == 'n')
            printf("%lx %s\n", addr, "n");          // Print and test if file is read correctly
        // DOING SIMULATION!
        ++numPredictions;
        if(!mainBP.makePrediction(addr,outcome)) ++numMispredictions;
    }
    
    predRate = (float)numPredictions / numMispredictions;
    printf("Results:\n");
    printf("Predictions: %d\n",numPredictions);
    printf("Mispredictions: %d\n",numMispredictions);
    printf("Prediction Correct Rate: %f\n",predRate);
    
    return 0;
}

branchPredictor::branchPredictor(int mInput, int nInput){
    m = mInput;
    n = nInput;
    size = pow(2,m);
    storage = new twoBitCounter[size];
    
    for(int i = 0; i < size; ++i){
        storage[i].val = 2;
        storage[i].index = 0;
    }
};

branchPredictor::~branchPredictor(){
    delete [] storage;   
}

int branchPredictor::makePrediction(unsigned long int addr,char outcome){
    int validIndex = -1;
    int correct;
    
    //trying to find past entry in 2 bit counter
    for(int i = 0; i < size; ++i){
        if (addr != storage[i].index) continue;
        else{
            validIndex = i;
            break;
        }
    }
    
    //if not found, populate in nearest empty slot
    if(validIndex < 0){
        for(int i = 0; i < size; ++i){
            if(storage[i].index == 0){
               storage[i].index = addr;
                validIndex = i;
            }
        }
    }
    
    //now that we have index, make prediction
    //make adjustment on outcome
    if(storage[validIndex].val >= 2){
        //WE PREDICT TAKE!   
        
        if(outcome == 't'){
            //CORRECT!
            if(storage[validIndex].val == 3) ++storage[validIndex].val;
            correct = 1;
        }
        
        else{
            //WRONG!
            --storage[validIndex].val;
            correct = 0;
        }
    }
    else{
        //WE PREDICT NO-TAKE!   
        
        if(outcome == 'n'){
            //CORRECT!
            if(storage[validIndex].val == 1) --storage[validIndex].val;
            correct = 1;
        }
        
        else{
            //WRONG!
            ++storage[validIndex].val;
            correct = 0;
        }
        
    }
    
    //print result and return (in)correct
    
    if(correct) printf("Made CORRECT prediction on %d, new val: %d\n",addr,storage[validIndex].val);
    else printf("Made INCORRECT prediction on %d, new val: %d\n",addr,storage[validIndex].val);
    
    return correct;
};


