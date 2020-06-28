#include "page.h"
#include "process.h"
#include <string>
#include <typeinfo>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <sys/types.h>

#define MEMORY 1024

using namespace std;

vector<vector<int>> processSize;
vector<vector<int>> trace;
deque<deque<Page *>> mainMemory;
vector<Process *> processList;
vector<vector<Page *>> virtualMemory;
int pageSwap = 0;

bool isInMemory(int processNum, int pref)
{
	int bit = virtualMemory[processNum][pref]->validBit;
	if (bit == 0)
	{
		return false;
	}
	return true;
}

void setReplaced0(Page *p)
{
	p->validBit = 0;
}

void FIFO(int frameSize, int pageSize, int tnum)
{
	int pid = 0;
	int pref = 0;
	for (int i = 0; i < tnum; i++)
	{
		cout << "Loop i: " << i << endl;
		pid = trace[i][0];
		pref = trace[i][1];
		Page *nextPage = virtualMemory[pid][pref / pageSize];
		if (nextPage->validBit == 0)
		{
			cout << "Not in Memory" << endl;
			if (mainMemory[pid].size() < frameSize)
			{
				mainMemory[pid].push_back(nextPage);
				cout << "Free space in memory" << endl;
			}
			else
			{
				cout << "Attempting to replace..." << endl;
				setReplaced0(mainMemory[pid].front());
				mainMemory[pid].pop_front();
				mainMemory[pid].push_back(nextPage);
				pageSwap++;
				cout << "Replaced" << endl;
			}
			nextPage->validBit = 1;
		}
	}
}

int getSeqPage(int pid, int pref, int pageSize)
{
	bool found = false;
	int index = pref / pageSize + 1;
	int processSize = processList[pid]->size;
	while (found == false)
	{
		if (index > processSize / pageSize)
		{
			return -1;
		}
		if (virtualMemory[pid][index]->validBit == 0)
		{
			found = true;
			return index;
		}
		index++;
	}
	return 0;
}

bool exceedLimit(int pid, int pref, int pageSize)
{
	return ((pref / pageSize + 1) > processList[pid]->size / pageSize);
}

int main(int argc, char *const argv[])
{

	int sizeOfPages;
	bool prepage;
	string flag;
	string prAlgo;
	string Algo;
	istringstream iss;

	if (argc != 5)
	{
		cerr << "Error: Please enter the correct amount of arguments" << endl;
		return 1;
	}
	ifstream in_1(argv[1]);
	if (!(in_1))
	{
		cerr << "Error: Please input a valid plist file" << endl;
		return 1;
	}
	int proNum, proMem;
	while (in_1 >> proNum >> proMem)
	{
		vector<int> pro;
		pro.push_back(proNum);
		pro.push_back(proMem);
		processSize.push_back(pro);
	}
	cout << "end" << endl;
	ifstream in_2(argv[2]);
	if (!(in_2))
	{
		cerr << "Error: Please input a valid ptrace file" << endl;
		return 1;
	}

	while (in_2 >> proNum >> proMem)
	{
		vector<int> tpro;
		tpro.push_back(proNum);
		tpro.push_back(proMem);
		trace.push_back(tpro);
	}

	sizeOfPages = atoi(argv[3]);
	cout << "Debug: size of Pages = " << sizeOfPages << endl;

	if (sizeOfPages <= 0)
	{
		cerr << "Error: Please enter a positive integer for size of pages" << endl;
		return 1;
	}
	cout << "Debug: Reach arg 4." << endl;
	iss.str(argv[4]);
	if (!(iss >> prAlgo))
	{
		cerr << "Error: Please enter a valid argument for page replacement algorithm" << endl;
		return 1;
	}
	iss.clear();
	for (int i = 0; i < prAlgo.length(); i++)
	{
		Algo += tolower(prAlgo[i]);
	}
	if ((Algo != "fifo") && (Algo != "lru") && (Algo != "clock"))
	{
		cerr << "Error: Please enter a valid page replacement algorithm to use" << endl;
		return 1;
	}

	int pnum = processSize.size();
	int tnum = trace.size();
	cout << "Debug: Reach 1st for-loop." << endl;
	cout << "Debug: Creating page tables" << endl;

	for (int i = 0; i < pnum; i++)
	{
		proMem = processSize[i][1];

		processList.push_back(new Process(i, proMem, sizeOfPages));
	}

	cout << "Debug: Reach 2nd for-loop: Initialize Virtual Memory" << endl;
	for (int i = 0; i < pnum; i++)
	{
		for (int j = 0; j < MEMORY / sizeOfPages / pnum; j++)
		{
			processList[i]->pageTable[j]->validBit = 1;
		}
		virtualMemory.push_back(processList[i]->pageTable);
	}

	cout << "Test: Iterate though virtual memory to print all the pageName" << endl;
	cout << "Debug: Reach 3rd for-loop: Initialize Main Memory" << endl;

	int memSize = MEMORY / sizeOfPages;
	int numProcesses = pnum;
	int numFrames = memSize / numProcesses;
	for (int i = 0; i < numProcesses; i++)
	{
		deque<Page *> processFrames;
		for (int j = 0; j < numFrames; j++)
		{
			processFrames.push_back(virtualMemory[i][j]);
		}
		mainMemory.push_back(processFrames);
		processFrames.clear();
	}

	cout << "Debug: Reach Algo Check." << endl;

	FIFO(numFrames, sizeOfPages, tnum);

	cout << "Number of page swaps for algorithm: " << Algo << " of page size " << sizeOfPages << " with prepaging " << flag << ": " << pageSwap << endl;
}
