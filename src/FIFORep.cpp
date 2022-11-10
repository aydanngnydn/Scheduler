/* @Author
Student Name: Aydan Günaydın
Student ID : 150200012
Date: 26/10/2022
*/
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "ProcessRep.h"
#include "FIFORep.h"

using namespace std;

FIFORep::FIFORep()
{
    this->mpHead = NULL;
    this->mpTail = NULL;
}

FIFORep::FIFORep(ProcessRep *head)
{
    this->mpHead = head;
    this->mpTail = head;
}

FIFORep::~FIFORep()
{
    ProcessRep *p = mpHead;
    ProcessRep *n = mpHead;

    while (n != NULL)
    {
        n = p->getNext();
        delete p;
        p = n;
    }
}

void FIFORep::setHead(ProcessRep *head)
{
    this->mpHead = head;
}

ProcessRep *FIFORep::getHead()
{
    return this->mpHead;
}

void FIFORep::setTail(ProcessRep *tail)
{
    this->mpTail = tail;
}

ProcessRep *FIFORep::getTail()
{
    return this->mpTail;
}

void FIFORep::queue(ProcessRep *p)
{
    if (mpHead == NULL)
    {
        mpHead = p;
        mpTail = p;
    }
    else
    {
        mpTail->setNext(p);
        setTail(p);
    }
}

ProcessRep *FIFORep::dequeue()
{
    ProcessRep *firstProcess = mpHead; // to keep the process that will be returned
    if (mpHead != NULL)
    {
        if (mpHead->getNext() != NULL)
        {
            mpHead = mpHead->getNext();
        }
        else
        {
            mpHead = NULL;
            mpTail = NULL;
        }
        firstProcess->setNext(NULL);
    }
    return firstProcess;
}

ProcessRep *FIFORep::searchID(int id)
{
    ProcessRep *currentNode = mpHead; // variable used for searching through list

    while (currentNode->getProcessID() != id)
    {
        if (currentNode->getNext() == NULL)
            return NULL;
        currentNode = currentNode->getNext();
    }
    return currentNode;
}

void FIFORep::printFIFO()
{
    if (mpHead != NULL)
    {
        ProcessRep *temp = mpHead; // variable used for printing through list

        while (temp != NULL)
        {
            cout << temp->getProcessType() << temp->getProcessID() << " " << temp->startTime << " " << temp->endTime << " ";
            temp = temp->getNext();
        }
    }
    else
    {
        return;
    }
}
