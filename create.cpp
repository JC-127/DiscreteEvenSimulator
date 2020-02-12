#include "des.h"
#include <stdlib.h>

//function creates a process, with time, event and PID as variables
Event createPro(int PID, int time, int event) 
{
	// new process to return
	Event nPro;

	// set time to desired time
	nPro.Time = time;

	// set to desired PID
	nPro.proID = PID;

	//sets the current state of the event, like what type of event it is.
	nPro.EveType = event;

	//returns the new process to be used in the main.
	return nPro;

} //end createPro