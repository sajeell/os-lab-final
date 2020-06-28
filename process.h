#ifndef PROCESS_H_
#define PROCESS_H_

#include "page.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Process {

public:
    int pid;
    vector<Page*> pageTable;
    int size; 
    
    Process (int x, int s, int sizeOfPages) {
        pid = x;
        size = s;
        for (int i = 0; i <= size/sizeOfPages; i++) {
            Page* newPage = new Page(pid, i, 0, 0);
            pageTable.push_back(newPage);
        }
    }
	
    Page* getPage(int i, int sizePages) {
        int pageNum = i/sizePages;
        return pageTable[pageNum];
    }    
};

#endif 
