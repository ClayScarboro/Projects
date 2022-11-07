#ifndef SIM_BP_H
#define SIM_BP_H

typedef struct bp_params{
    unsigned long int K;
    unsigned long int M1;
    unsigned long int M2;
    unsigned long int N;
    char*             bp_name;
}bp_params;

class BHT {
public:
    int m,n;
    
    BHT(int m,int n);
    
    void editBHT();
    
};

#endif
