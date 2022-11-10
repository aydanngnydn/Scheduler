/* @Author
Student Name: Aydan Günaydın
Student ID : 150200012
Date: 26/10/2022
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
// #include "node.h"
#include "ProcessRep.h"
#include "FIFORep.h"
#include "CPURep.h"

using namespace std;

CPURep::CPURep()
{
    this->mFinishedProcess = new FIFORep;
}

CPURep::~CPURep()
{
    delete mFinishedProcess;
}

ProcessRep *CPURep::runCPU(ProcessRep *p, int time)
{
    p->remainingTime -= 1;
    if (p->remainingTime == 1)//I control if it is 1 because print function is called before schedule function and schedule reduces remaining time after printing but in the same time count
    {
        p->endTime = time + 1;//print function is called before schedule function so i have to add 1 to show whats happening in the running time count
        if (getFinishedProcess() == NULL)//if finished process fifo is not created
        {
            setFinishedProcess(new FIFORep(p));
        }
        mFinishedProcess->queue(p);
        return NULL; // to communicate with schedule function
    }
    return p;
}

FIFORep *CPURep::getFinishedProcess()
{
    return mFinishedProcess;
}

void CPURep::setFinishedProcess(FIFORep *finishedProcess)
{
    this->mFinishedProcess = finishedProcess;
}