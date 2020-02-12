#include <stdlib.h>
#include <string>
#include <queue>

#ifndef DES_H_
#define DES_H_

using namespace std;

//VARIABLES AND THEIR PRIORITY NUMBERS
int const PRO_ARRIV = 0;
int const CPU_ENTER = 1;
int const CPU_FIN = 2;
int const PROCESS_EXIT = 3;
int const D1_START = 4;
int const D2_START = 5;
int const D1_FIN = 6;
int const D2_FIN = 7;
int const SIMULATION_FINISHED = 8;

//Structure of what an event is
struct Event {

    int proID;   // ID of a process

    int Time;   //Time of event

    int EveType; //Event Type

    //sets the priority of the event
    bool operator<(const Event& rhs) const
    {
        return Time > rhs.Time;
    }//end bool()
}; //end struct Event{}


//DECLARED FUNCTIONS
//Function Calls to initialize the values to 0
int getSEED();
int getINIT_TIME();
int getFIN_TIME();
int getCPU_MIN();
int getCPU_MAX();
int getARRIVE_MIN();
int getARRIVE_MAX();
int getDISK1_MIN();
int getDISK1_MAX();
int getDISK2_MIN();
int getDISK2_MAX();
float getQUIT_PROB();

// EVENT HANDLERS
int Update_CPU(priority_queue<Event>& eq, queue <Event>& CPUQ, int& CPU_IDLE, int currTime, FILE* Out);
int handleCPUfinish(int counter, priority_queue<Event>& eq, queue <Event>& D1Q, queue <Event>& D2Q, int& CPU_IDLE, int currTime, FILE* Out);
int HandleArrival(int counter, priority_queue<Event>& eq, queue <Event>& CPUQ, int currTime, FILE* Out, FILE* Stat);
int UPDATE_DISK(priority_queue<Event>& eq, queue <Event>& D1Q, queue <Event>& D2Q, int& D2_IDLE, int& D1_IDLE, int currTime, FILE* Out);
int handleDISKfinish(priority_queue<Event>& eq, int currTime, FILE* Out);

//UTILITIES
Event createPro(int PID, int time, int event);      //function that creates a new process;
int PriEQ(priority_queue<Event> que);                  
int ranBool(float prb);
string* Init(FILE* Out);
int ranRange(int min, int max);
float ranF();
int PriSt(int FIN_Time, int JNum);                //Logs what happens
int initRan(int seed);
int printUtil(FILE* Stat, int FIN_TIME, int JNum);

//get functions
int getCPU();
int getD1();
int getD2();

#endif