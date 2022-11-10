/* @Author
Student Name: Aydan Günaydın
Student ID : 150200012
Date: 26/10/2022
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "SchedulerRep.h"

using namespace std;

SchedulerRep::SchedulerRep()
{
    this->mpProcessFIFO[0] = new FIFORep;
    this->mpProcessFIFO[1] = new FIFORep;
    this->mpProcessFIFO[2] = new FIFORep;
    this->mpRunningProcess = NULL;
    this->timeSliceCount = 0;
    this->pCpuObj = new CPURep();
    this->totalTime = 0;
}
SchedulerRep::~SchedulerRep()
{
    delete pCpuObj;
    delete mpProcessFIFO[0];
    delete mpProcessFIFO[1];
    delete mpProcessFIFO[2];
}

FIFORep *SchedulerRep::getProcessFIFO(int index)
{
    return this->mpProcessFIFO[index];
}

void SchedulerRep::setProcessFIFO(FIFORep *fifo, int index)
{
    this->mpProcessFIFO[index] = fifo;
}

void SchedulerRep::setRunningProcess(ProcessRep *p)
{
    this->mpRunningProcess = p;
}

ProcessRep *SchedulerRep::getRunningProcess()
{
    return this->mpRunningProcess;
}

void SchedulerRep::pushProcess(ProcessRep *p)
{
    if (p->getProcessType() == "A")
    {
        this->mpProcessFIFO[0]->queue(p);
    }
    else if (p->getProcessType() == "B")
    {
        this->mpProcessFIFO[1]->queue(p);
    }
    else if (p->getProcessType() == "C")
    {
        this->mpProcessFIFO[2]->queue(p);
    }
}

ProcessRep *SchedulerRep::popProcess()
{
    ProcessRep *removedProcess = NULL;
    if (mpProcessFIFO[0]->getHead() != NULL)
    {
        removedProcess = mpProcessFIFO[0]->dequeue();
    }
    else if (mpProcessFIFO[1]->getHead() != NULL)
    {
        removedProcess = mpProcessFIFO[1]->dequeue();
    }
    else if (mpProcessFIFO[2]->getHead() != NULL)
    {
        removedProcess = mpProcessFIFO[2]->dequeue();
    }
    return removedProcess;
}

bool SchedulerRep::checkTimeSlice()
{
    int quantumTime;
    if (mpRunningProcess->getProcessType() == "A")
        quantumTime = 2;
    else if (mpRunningProcess->getProcessType() == "B")
        quantumTime = 4;
    else if (mpRunningProcess->getProcessType() == "C")
        quantumTime = 8;

    if (quantumTime == timeSliceCount)
    {
        timeSliceCount = 0;
        return true;
    }
    else
    {
        return false;
    }
}

ProcessRep *SchedulerRep::sendProcessToCPU(ProcessRep *p)
{
    if (mpRunningProcess == NULL) // to prevent time decreasment for the first enter
    {
        mpRunningProcess = p;
        if (p->startTime == -1) // default startTime is -1 so I control if start time is not set
        {
            p->startTime = totalTime;//startTime equals currentTime
        }
    }
    else
    {
        if (p->remainingTime != 1) // not 0 because next line decreases remaining time and process may decrease to 0 so I control it beforehand
        {
            if (pCpuObj->runCPU(p, totalTime) == NULL) // to receive the runcpu's return value
            {
                return NULL;//process is finished so I return NULL to control it in schedule function
            }

            timeSliceCount++;
        }
        else
        {
            setRunningProcess(popProcess());//remaining time is 1 so process is about to finish

            if (pCpuObj->runCPU(mpRunningProcess, totalTime) == NULL)// to receive the runcpu's return value
            {
                return NULL;//process is finished so I return NULL to control it in schedule function
            }
        }
    }
    return p;
}
void SchedulerRep::schedule(string type, int id, int arrivalTime, int processTime)
{
    ProcessRep *newProcess = new ProcessRep(type, id, arrivalTime, processTime);

    if (getRunningProcess() == NULL)//if there is no running process
    {
        sendProcessToCPU(newProcess);
    
    }
    else
    {
        if (sendProcessToCPU(mpRunningProcess) == NULL) // controlling if process has finished
        {
            setRunningProcess(popProcess());          // setting new process
            getRunningProcess()->remainingTime++;     //remaining time is one more because remaining time decreaes before next time period but it shouldnt in the first run
            if (getRunningProcess()->startTime == -1) //if starttime is not assigned
            {
                getRunningProcess()->startTime = totalTime + 1;// runningprocess changes when process finishes so I increase it here to show it when printing
            }
            timeSliceCount = -1;//timeslicecount increases when next process first enters cpu so its timeslice starts from -1
        }

        if (checkTimeSlice() && mpRunningProcess->remainingTime != 1)// not 0 because int this time period remaining time will decrease and process may decrease to 0 so I control it beforehand and give priority to check if process is already finished
        {
            pushProcess(newProcess);

            pushProcess(mpRunningProcess);

            setRunningProcess(NULL);

            sendProcessToCPU(popProcess());
            
        }

        else if (type < this->getRunningProcess()->getProcessType())
        {
            timeSliceCount = 0;//running process will change so timesliecount has to start from 0

            pushProcess(newProcess);

            pushProcess(mpRunningProcess);

            setRunningProcess(NULL);

            sendProcessToCPU(popProcess());
        }

        else if (type >= this->getRunningProcess()->getProcessType())
        {
            pushProcess(newProcess);
        }
    }
    totalTime++;
}

void SchedulerRep::schedule(ProcessRep *p)
{
    if (getRunningProcess() != NULL)
    {
        if (sendProcessToCPU(mpRunningProcess) == NULL)
        {
            setRunningProcess(popProcess());

            if (getRunningProcess() == NULL)
            {
                totalTime++;
                return;
            }
            getRunningProcess()->remainingTime++;
            if (getRunningProcess()->startTime == -1)//if starttime is not assigned
            {
                getRunningProcess()->startTime = totalTime + 1;// runningprocess changes when process finishes so I increase it here to show it when printing
            }
            timeSliceCount = -1; // timeslicecount increases when next process first enters cpu so its timeslice starts from -1
        }

        if (checkTimeSlice() && getRunningProcess()->remainingTime > 1)// not 0 because int this time period remaining time will decrease and process may decrease to 0 so I control it beforehand and give priority to check if process is already finished
        {
            pushProcess(mpRunningProcess);

            setRunningProcess(NULL);

            sendProcessToCPU(popProcess());
        }
    }
    totalTime++;
}
