#include "des.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
using namespace std;

int ranRange(int min, int max)
{
    //random number that will be returned;
    int rNum;
    rNum = (rand() % (max + min));
    return rNum;

}//end ranRange()

//function for the srand class too determine process quit & disk probabilities
float ranF() 
{

    float float1;
    float1 = rand() / (double(RAND_MAX));
    return float1;
} // end randomFloat

//Returns random boolean and takes a float integer which will represent the proability
int ranBool(float prb) 
{

    int reVal;      //this will be returned hence why its called reVal or "Returned Value"
    float temp1;
    temp1 = ranF();
    
    //if the temporary assigned value is less than the probability, then its true
    if (temp1 < prb)
    {
        reVal = 0;
        return  reVal;
    }//end if()

    //if it aint true it must be.....thats right, YOU ARE NOT THE FATHER. (False)
    else 
    {
         reVal = 1;
        return  reVal;
    }//end else()

}//end ranBool()

//for randomization for the random number generator
int initRan(int seed) 
{
    int temp2 = seed;
    srand(seed);
    return 0;
}//end initRan()