#pragma once
#include <ctime>
#include <chrono>

#include "piece.h"

enum Mode { fixedDepth, timed };

class timeManagement
{
public:
	timeManagement();
	timeManagement(long int wtime, long int btime, colours colour);
	timeManagement(int depth);

	~timeManagement();

	bool isMoreTime(int searchDepth);

private:
	std::chrono::high_resolution_clock::time_point startTime;
	long int targetTime;
	Mode searchMode;
	int targetDepth;
};


