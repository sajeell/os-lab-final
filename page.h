#ifndef PAGE_H_
#define PAGE_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Page
{

public:
    int pid;
    int pageid;
    int validBit;

    int lastTimeAccessed;

    Page(int pid, int pageid, int validBit, int rBit)
    {
        this->pid = pid;
        this->pageid = pageid;
        this->validBit = validBit;
    }

    bool isValid()
    {
        if (validBit == 0)
        {
            return false;
        }
        return true;
    }
};

#endif /* PAGE_H_ */
