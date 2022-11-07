#ifndef SIM_BP_H
#define SIM_BP_H

typedef struct bp_params{
    unsigned long int K;
    unsigned long int M1;
    unsigned long int M2;
    unsigned long int N;
    char*             bp_name;
}bp_params;

class twoBitCounter{
public:
    int val;
    int index;
};

class branchPredictor{
public:
    int m, n, size;
    twoBitCounter *storage = NULL;
    
    branchPredictor(int m,int n);
    ~branchPredictor();
    int makePrediction(unsigned long int addr,char outcome);
    
};


#endif
