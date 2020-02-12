#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include "des.h"

using namespace std;


//QUEUES
priority_queue <Event> EQ;            // Creates a priority queue
queue <Event> CPU_Q;               //create CPU FIFO queue
queue <Event> D1Q;           // create DISK 1 FIFO queue
queue <Event> D2Q;           // create DISK 2 FIFO queue

//self explanatory
int currTime;

//IDLE NOTIFIERS
int CPU_IDLE;
int D1_IDLE;
int D2_IDLE;
int Processing;

int EQMax;             // highest number of elements in event queue
int MAX_CPU;            // highest number of elements in CPU queue
int D1_MAX;          // highest number of elements in Disk1 queue
int D2_MAX;          // highest number of elements in Disk2 queue
int EQ_TOT;           // to calculate avg # of elements in queue
int CPU_TOT;          // to calculate avg # of elements in queue
int D1_TOT;        // to calculate avg # of elements in queue
int D2_TOT;         // to calculate avg # of elements in queue

FILE* Out;                //log file
FILE* St;               //stat file

int main() 
{
    // set flags for modules to all be idle on start
    CPU_IDLE = 0;
    D1_IDLE = 0;
    D2_IDLE = 0;
    Processing = 0;

    // initialize counters
    EQMax = 0;
    MAX_CPU = 0;
    D1_MAX = 0;
    D2_MAX = 0;
    EQ_TOT = 0;
    CPU_TOT = 0;
    D1_TOT = 0;
    D2_TOT = 0;

    // naming out put files
    char lFi[] = "log.txt";
    char sFi[] = "Stat.txt";


    //create array for parsing config file
    string* IniDa = new string[1000];

    // delete old log.txt file every time we run
    //Deletes out.txt file if exists
    if (remove(lFi) != 0)
        perror("log File deletion failed");
    else
        cout << "log File deleted successfully" << endl;

    //delete old Stat.txt file every time we run
    //Deletes out.txt file if exists
    if (remove(sFi) != 0)
        perror("Stat File deletion failed");
    else
        cout << "Stat File deleted successfully" << endl;

    //make a new file called
    Out = fopen("log.txt", "a");

    //make a new file called 
    St = fopen("Stat.txt", "a");

    //Initialize an array called IniDa from file 'confi'
    IniDa = Init(Out);

    //Start timer after getting the file 'confi'
    int time = getINIT_TIME();

    //begin generating random numbers
    initRan(getSEED());

    //Establish the priorities between the events
    Event job0 = createPro(0, 1, PRO_ARRIV);
    Event EndSimulation = createPro(2100, getFIN_TIME(), SIMULATION_FINISHED);
    Event thirdProc = createPro(2, 3, PRO_ARRIV);
    EQ.push(job0);                                // adds the first job
    EQ.push(thirdProc);                           // adds the third job
    EQ.push(EndSimulation);			             // adds the last job

    int counter = 3;

    //MAIN WHILE LOOP FOR THE WHOLE SIMULATION TO RUN
    //while queue has no jobs or event pending and last event has not arrived
    while (Processing == 0) 
    {

        //sets time iteration through loop to get the events and jobs to run.
        currTime = EQ.top().Time;

        //finds the maximum size of each queue.
        if (EQ.size() >= EQMax) { EQMax = EQ.size(); cout << EQ.size() << endl; }
        if (CPU_Q.size() >= MAX_CPU) { MAX_CPU = CPU_Q.size(); }
        if (D1Q.size() >= D1_MAX) { D1_MAX = D1Q.size(); }
        if (D2Q.size() >= D2_MAX) { D2_MAX = D2Q.size(); }

        //Totals of each device
        EQ_TOT += EQ.size();
        CPU_TOT += CPU_Q.size();
        D1_TOT += D1Q.size();
        D2_TOT += D2Q.size();
            
        //prioritize the events if the event has a high priority then the other
        switch (EQ.top().EveType) 
        {
            //CASES OF THE EVENTS
            case PRO_ARRIV: 
            {
                HandleArrival(counter, EQ, CPU_Q, currTime, Out, St);
                counter++;
            }//end case PRO_ARRIV

            break;
                        
            case CPU_ENTER: 
            {
                EQ.pop();
                counter++;
            }//end case CPU_ENTER

            break;
                        
            case CPU_FIN: 
            {
                CPU_IDLE = 0;
                handleCPUfinish(counter, EQ, D1Q, D2Q, CPU_IDLE, currTime, Out);
                counter++;
            }//end case 2

            break;
                    
            case PROCESS_EXIT: 
            {
                printf("TIME:%i JOB:%i EXITED\n", currTime, EQ.top().proID);
                fprintf(Out, "TIME:%i JOB:%i EXITED\n", currTime, EQ.top().proID);
            
                EQ.pop();
                counter++;
            }//end case

            break;
                    
            case D1_START: 
            {
                EQ.pop();
                counter++;
            }//end 
                
            break;
                   
            case D2_START: 
            {
                EQ.pop();
                counter++;
            }//end case
                  
            break;
                     
            case D1_FIN:
            {
                D1_IDLE = 0;
                handleDISKfinish(EQ, currTime, Out);
                counter++;
            }//end case
        
            break;
                   
            case D2_FIN:
            {
                D2_IDLE = 0;
                handleDISKfinish(EQ, currTime, Out);
                counter++;
            }//end 

            break;
            
            case SIMULATION_FINISHED:
            {
                Processing = 1;
                printf("Time:%i Job:%i Simulation has ended...\n", currTime, EQ.top().proID);
                fprintf(Out, "Time:%i Job:%i Simulation has ended...\n", currTime, EQ.top().proID);

                //print stats before finishing
                PriSt(getFIN_TIME(), getINIT_TIME());
                fclose(Out);
                fclose(St);
            
                return 0;
            
                counter++;
            }//end

            break;
            default:
            break;
             
        } // end switch

        //UPDATE CPU AND DISK
        Update_CPU(EQ, CPU_Q, CPU_IDLE, currTime, Out);
        UPDATE_DISK(EQ, D1Q, D2Q, D2_IDLE, D1_IDLE, currTime, Out);

    }//end while()

    fclose(Out);
    fclose(St);
    return 0;

}//end main()

// function to use in testing to print event queue
// this function copy's event queue and prints out the copy
int PriEQ(priority_queue<Event> Que) 
{
    Event TempEvent;

    while (!Que.empty())
    {
        // check top
        TempEvent = Que.top();
        printf("EQ ID:%i Time:%i Event: %i \n", TempEvent.proID, TempEvent.Time, TempEvent.EveType);

        Que.pop();

    } // end while

    return 0;

} // end Print EQ

//function to call to print out stat file
int PriSt(int FIN_TIME, int INIT)
{

    int someNumber = INIT;
    fprintf(St, "Stat File\n\n");
    fprintf(St, "Sizes of Queues\n");
 
    fprintf(St, "Maximum size of Event: %i \n", EQMax);
    fprintf(St, "Average size of Event: %i \n\n", EQ_TOT / getFIN_TIME());
    
    fprintf(St, "Maximum size of CPU: %i \n", MAX_CPU);
    fprintf(St, "Average size of CPU: %i \n\n", CPU_TOT / getFIN_TIME());
    
    fprintf(St, "Maximum size of Disk 1: %i \n", D1_MAX);
    fprintf(St, "Average size of Disk 1: %i \n\n", D1_TOT / getFIN_TIME());
    
    fprintf(St, "Maximum size of Disk 2: %i \n", D2_MAX);
    fprintf(St, "Average size of Disk 2: %i \n\n", D2_TOT / getFIN_TIME());
   
    fprintf(St, "UTILIZATION\n\nTOTAL SIMULATION RUN TIME: %i\n\n", FIN_TIME - getINIT_TIME());

    printUtil(St, 8000, someNumber);

    return 0;

}//end print stats
