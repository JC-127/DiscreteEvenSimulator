#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "des.h"


using namespace std;

// array to store data from config file
string* InitData = new string[1000];

int NumOfCon = 12; 
int SEED;
int INIT_TIME;
int FIN_TIME;
int ARRIVE_MIN;
int ARRIVE_MAX;
float QUIT_PROB;
int CPU_MIN;
int CPU_MAX;
int DISK1_MIN;
int DISK1_MAX;
int DISK2_MIN;
int DISK2_MAX;


string* Init(FILE* OutTXT) 
{
    int counter = 0;                             //its just a counter
    string tracker;                                 //this is a string that will be used to keep track of the file, tracker by tracker
    ifstream file("conf.txt");              //this is the command the searches for the file. 
    
    if (file.is_open())
    {
        // split each tracker
        while (getline(file, tracker))
        {
            InitData[counter] = tracker;
            counter++;
        } // end while


        for (int i = 0; i < NumOfCon; i++)
        {
            // split first words
            string Word1 = InitData[i].substr(0, InitData[i].find(" "));
            //position of blank space
            size_t whitespace = InitData[i].find(" ");
            // second word
            string Word2 = InitData[i].substr(whitespace);
            //make array of  second  values
            InitData[i] = Word2;
            printf(" Loading Data from file 'Conf' %s %s \n", Word1.c_str(), Word2.c_str());
            fprintf(OutTXT, "Loading Data from file 'Conf' %s %s \n", Word2.c_str(), Word2.c_str());

        } // end for

        cout << endl << endl;
        fprintf(OutTXT, "\n\n");

        // convert strings to int
        SEED = atoi(InitData[0].c_str());
        INIT_TIME = atoi(InitData[1].c_str());
        FIN_TIME = atoi(InitData[2].c_str());
        ARRIVE_MIN = atoi(InitData[3].c_str());
        ARRIVE_MAX = atoi(InitData[4].c_str());
        QUIT_PROB = atof(InitData[5].c_str());    //This is a float, it will not convert a float to a string
        CPU_MIN = atoi(InitData[6].c_str());
        CPU_MAX = atoi(InitData[7].c_str());
        DISK1_MIN = atoi(InitData[8].c_str());
        DISK1_MAX = atoi(InitData[9].c_str());
        DISK2_MIN = atoi(InitData[10].c_str());
        DISK2_MAX = atoi(InitData[11].c_str());
                     
        file.close();

    } // end if

    else
    {
        cout << "Unable to open file";
    }//end else()

    return InitData;

}//end 

//functions that return values to the main
int getSEED() { return SEED; };
int getINIT_TIME() { return INIT_TIME; };
int getFIN_TIME() { return FIN_TIME; };
int getARRIVE_MIN() { return ARRIVE_MIN; };
int getARRIVE_MAX() { return ARRIVE_MAX; };
float getQUIT_PROB() { return QUIT_PROB; };
int getCPU_MIN() { return CPU_MIN; };
int getCPU_MAX() { return CPU_MAX; };
int getDISK1_MIN() { return DISK1_MIN; };
int getDISK1_MAX() { return DISK1_MAX; };
int getDISK2_MIN() { return DISK2_MIN; };
int getDISK2_MAX() { return DISK2_MAX; };