#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include "des.h"


using namespace std;

//counters for their times
int CPU_BT;       //counter for CPU 
int D1BTime;     //counter for Disk 1
int D2BTime;     //counter for Disk 2

//Jobs for the Queue.
int CPUJ;
int D1J;
int D2J;

//Time lof of the each of the devices
int CPU_MT = 0;
int D1_MT = 0;
int D2_MT = 0;
int JTimer;



//Arrival Handler
int HanArri(int counter, priority_queue<Event>& EQ, queue <Event>& CPUQ, int currTime, FILE* Out, FILE* Stat) 
{
    //creates a new event for cpu queue with same PID as the EQ queue and says it has entered the queue.
    Event tempEvent = createPro(EQ.top().proID, currTime, CPU_ENTER);

    EQ.pop();

    printf("Time:%i Job:%i Has Entered the CPU queue\n", currTime, tempEvent.proID);
    fprintf(Out, "Time:%i Job:%i  Has entered the CPU queue\n", currTime, tempEvent.proID);

    // push top of the event queue to cpu queue
    CPUQ.push(tempEvent);
    JTimer = counter;
    
    //Initialize all the values if the current time has started
    if (currTime <= 1) 
    { 
        CPU_BT = 0; 
        D1BTime = 0; 
        D2BTime = 0; 
        CPU_MT = 0; 
        D1_MT = 0; 
        D2_MT = 0; 
        CPUJ = 0; 
        D1J = 0; 
        D2J = 0; 
    }//end if()
    
    return 0;

};

//UPDATE CPU
int U_CPU(priority_queue<Event>& EQ, queue <Event>& CPUQ, int& CPU_IDLE, int currTime, FILE* Out)
{
    //If CPU is not busy and there is something needed to be processed
    if ((CPU_IDLE == 0) && !CPUQ.empty()) 
    {
        int range = ( ranRange (getCPU_MIN(), getCPU_MAX() ) );
       
        //Seatch for longest task and say thats the longest task
        if (range >= CPU_MT)
        {
            CPU_MT = range;
        }//end if()
         
        //Take the front element off the CPU queue --> to the event Queue for processing once CPU is done
        Event tempEvent = createPro(CPUQ.front().proID, currTime + range, CPU_FIN);

        printf("Time:%i Job:%i CPU Processing... \n", currTime, tempEvent.proID);
        fprintf(Out, "Time:%i Job:%i CPU Processing...\n", currTime, tempEvent.proID);
        
        CPUQ.pop();

        //Push event to event queue after CPU is done
        EQ.push(tempEvent);
        
        //Timer on how long the CPU is used.
        CPU_BT += range;
        CPU_IDLE = 1;
        
        return 1;

    }//end if()
  
    return 0;

}// end UPDATE_CPU

//HANDLE CPU FINISh
int hanCPUfin(int counter, priority_queue<Event>& EQ, queue <Event>& D1Q, queue <Event>& D2Q, int& CPU_IDLE, int currTime, FILE* Out) 
{

    int sizeof_D1 = D1Q.size();
    int sizeof_D2 = D2Q.size();
    int tempBool;
    Event tempEvent;
    
    //Probaility to quit process
    tempBool = ranBool(.5f);
    
    //This will be the time marker for the next event
    int newArrival;
    
    //next step in the process
    int nextEvent;

    if (ranF() < getQUIT_PROB())
    {
        //set next event to exit the process 
        nextEvent = PROCESS_EXIT;
       
        //New time creation for next event
        newArrival = currTime + ranRange(getARRIVE_MIN(), getARRIVE_MAX());
        
        //create a new event to exit the system with the existing PID , time, & event at Next Event
        tempEvent = createPro(EQ.top().proID, newArrival, nextEvent);
                
        EQ.pop();
        EQ.push(tempEvent);

        printf("Time:%i Job:%i is finished processing in CPU, Exiting system...\n", currTime, tempEvent.proID);
        fprintf(Out, "Time:%i Job:%i CPU is processing...\n", currTime, tempEvent.proID);
        CPU_IDLE = 0;
        CPUJ++;
    }//end 

    else 
    {
        // go to disk 1 if it has less stuff in it
        if (ranF() <= .5f)
        {

            nextEvent = D1_START;
            
            //New time for next event arrival
            newArrival = currTime + ranRange(getDISK1_MIN(), getDISK1_MAX());
            
            //Nnew event for exiting the system with existing PID , time, & event at Next Event
            tempEvent = createPro(EQ.top().proID, newArrival, nextEvent);
            
            EQ.pop();
            D1Q.push(tempEvent);
            
            printf("Time:%i Job:%i is finished processing in CPU, rerouting to Disk 1\n", currTime, tempEvent.proID);
            fprintf(Out, "Time:%i Job:%i is finised processing in CPU, Item was sent Disk 1\n", currTime, tempEvent.proID);
            
            CPUJ++;
        }//end

        else
        {
            nextEvent = D2_START;

            // create new time for next arrival event
            newArrival = currTime + ranRange(getDISK2_MIN(), getDISK2_MAX());
           
            // create a new event for adding to cpu queue , with same PID ,random time,and event at CPU_ENTER
            tempEvent = createPro(EQ.top().proID, newArrival, nextEvent);
                        
            EQ.pop();
            
            printf("Time:%i Job:%i is finished processig in CPU, rerouting to Disk 2\n", currTime, tempEvent.proID);
            fprintf(Out, "TIME:%i JOB:%i is finished processing in CPU, ITem was sent to Disk 2\n", currTime, tempEvent.proID);
            
            D2Q.push(tempEvent);
            
            CPUJ++;

        } // end else

    } //end else

    //Self explanatory at this point
    newArrival = currTime + ranRange(getARRIVE_MIN(), getARRIVE_MAX());
    
    //new job arrival event at random time
    Event newEvent = createPro(counter, newArrival, PRO_ARRIV);
    
    EQ.push(newEvent);
    
    printf("Time:%i Job:%i has arrrived at destination\n", currTime, newEvent.proID);
    fprintf(Out, "TIME:%i JOB:%i has arrived at destination\n", currTime, tempEvent.proID);

    CPU_IDLE = 0;
    return 0;

}//end hanCPUfin

//Disk Handler
int hanDISKfin(priority_queue<Event>& EQ, int currTime, FILE* Out)
{
    Event tempEvent;
    int newArrival1;
    int nextEvent;

    nextEvent = PRO_ARRIV;
 
    //new time for next arrival
    newArrival1 = currTime + ranRange(getARRIVE_MIN(), getARRIVE_MAX());
    
    //new event for CPU queue with existing PID, time, & event at CPU_ENTER
    tempEvent = createPro(EQ.top().proID, newArrival1, nextEvent);

    //check if the next event is a disk 1 finished event
    if (EQ.top().EveType == D1_FIN) 
    {
        
        printf("Time:%i Job:%i has finished in Disk 1, rerouting back to CPU...\n", currTime, tempEvent.proID);
        fprintf(Out, "Time:%i Job:%i has finished processing on Disk 1, item sent back to CPU\n", currTime, tempEvent.proID);
        D1J++;
    }//end

    // check if the next event is a disk 2 finished event
    else if (EQ.top().EveType == D2_FIN)
    {
        printf("Time:%i Job:%i has finishd on Disk 2, rerouting back to CPU\n", currTime, tempEvent.proID);
        fprintf(Out, "Time:%i Job:%i has finished on Disk 2, item sent back to CPU\n", currTime, tempEvent.proID);
        D2J++;
    }//end 

    // pop of top of event queue
    EQ.pop();

    //push to event queue
    EQ.push(tempEvent);



    return 0;

}// end handle DISK finish

//UPDATE DISK()
int U_DISK(priority_queue<Event>& EQ, queue <Event>& D1Q, queue <Event>& D2Q, int& D2_IDLE, int& D1_IDLE, int currTime, FILE* Out) 
{

    //Disk is not empty and something is in queue
    if ((D1_IDLE == 0) && !D1Q.empty()) 
    {

        int range = ranRange(getDISK1_MIN(), getDISK1_MAX());
        
        //search longest task
        if (range >= D1_MT)
        {
            D1_MT = range;
        }//end if()

        //new event with same id as front of disk 1 but with disk finished status and new time
        Event tempEvent = createPro(D1Q.front().proID, currTime + range, D1_FIN);
        printf("Time:%i Job:%i :Disk 1 Processing...\n", currTime, tempEvent.proID);
        fprintf(Out, "TIME:%i JOB:%i :Disk 1 Processing...\n", currTime, tempEvent.proID);

        D1Q.pop();
        EQ.push(tempEvent);
            
        //Disk processing time counter 
        D1BTime += range;

        printf("Time:%i JOB:%i Pushing to event queue from DISK 1 with as type %i \n", currTime, tempEvent.proID, tempEvent.EveType);
        D1_IDLE = 1;

    }//end

    //Idle will be logged 
    else if (D1_IDLE == 1)
    {
        cout << "DISK 1 IN USE " << endl; 
    }//end 

    //Disk is idle and there is something in the queue
    if ((D2_IDLE == 0) && !(D2Q.empty()))
    {

        int range = ranRange(getDISK2_MIN(), getDISK2_MAX());
        
        if (range >= D2_MT)
        {
            D2_MT = range;
        }//end if()

        Event tempEvent = createPro(D2Q.front().proID, currTime + range, D2_FIN);
        printf("Time:%i Job:%i :Disk 2 processig...\n", currTime, tempEvent.proID);
        fprintf(Out, "TIME:%i JOB:%i :Disk 2 processing...\n", currTime, tempEvent.proID);

        cout << D2Q.front().proID << "Popping off Queue of Disk 2" << endl;
        D2Q.pop();

        //Timer for Disk 2 being used
        D2BTime += range;
        
        EQ.push(tempEvent);
        printf("Time:%i Job:%i Pushing to event queue from Disk 2 with event type %i \n", currTime, tempEvent.proID, tempEvent.EveType);

        D2_IDLE = 1;

    }//end

    //self explanatory
    else if (D2_IDLE == 1)
    {
        cout << "DISK 2 IN USE " << endl;
    }//end

    return 0;

}// end UPDATE_DISK

//STATS
int printUtil(FILE* Stat, int FIN_TIME, int init)
{
    //total time from when we started
    float fin = (float)FIN_TIME;
    
    //Total time of utilization
    float totalTime = (FIN_TIME + 0.0) - (float)init;
    
    //total times of the hardware
    double CPUutil = ((float)CPU_BT) / totalTime;
    double D1 = ((float)D1BTime) / totalTime;
    double D2 = ((float)D2BTime) / totalTime;
    double EQjobs = ((double)CPUJ) + ((double)D1J) + ((double)D2J);
    

    double CPUthrough = ((double)CPUJ) / (float)CPU_BT + 0.0;
    double D1through = ((double)D1J) / (float)D1BTime + 0.0;
    double D2through = ((double)D2J) / (float)D2BTime + 0.0;
    // double EQthrough =  EQjobs / totalTime + 0.0f  ;

    //Utilizations
    fprintf(Stat, "CPU Utilization: %f.2%c \n", CPUutil, '%');
    fprintf(Stat, "Disk 1 Utilization: %f.2%c \n", D1, '%');
    fprintf(Stat, "Disk 2 Utilization: %f.2%c \n", D2, '%');
    
    fprintf(Stat, "\n");
    
    //jobs
    fprintf(Stat, "Total CPU jobs processed: %i \n", CPUJ);
    fprintf(Stat, "Total Disk 1 jobs processed: %i \n", D1J);
    fprintf(Stat, "Total Disk 2 jobs processed: %i \n\n", D2J);
   
    fprintf(Stat, "THROUGHPUT \n\n");
    //through put is jobs services/ time spent servicing jobs
       
    fprintf(Stat, "CPU Throughput: %.3f \n", CPUthrough);
    fprintf(Stat, "Disk 1 Throughput: %.6f \n", D1through);
    fprintf(Stat, "Disk 2 Throughput: %.6f \n\n", D2through);
    
    fprintf(Stat, "RESPONSE TIME \n\n");
    
    //longest jobs 
    fprintf(Stat, "Longest CPU job: %i \n", CPU_MT);
    fprintf(Stat, "Longest Disk 1 job: %i \n", D1_MT);
    fprintf(Stat, "Longest Disk 2 job: %i \n\n", D2_MT);

    float tempVal = ((float)CPU_BT + 0.0) / ((double)CPUJ);
      
    fprintf(Stat, "CPU average job: %f.4 \n", tempVal);
    tempVal = ((float)D1BTime + 0.0) / ((double)D1J);
    fprintf(Stat, "DISK1 average job: %f.4 \n", tempVal);
    tempVal = ((float)D2BTime + 0.0) / ((double)D2J);
    fprintf(Stat, "DISK2 average job: %f4.4 \n", tempVal);

    return 0;

}//end

//Function Calls
int getCPU() 
{
    int num = CPU_BT;
    return num;
}//end

int getD1() 
{
    int num = D1BTime;
    return num;
}//end

int getD2()
{
    int num = D2BTime;
    return num;
}//end 
