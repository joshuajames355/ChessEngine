#pragma once
#include <ctime>
#include <chrono>

#include "piece.h"

class timeManagement
{
public:
	timeManagement(long int wtime, long int btime, colours colour);
	~timeManagement();

	bool isMoreTime();

private:
	std::chrono::high_resolution_clock::time_point startTime;
	long int targetTime;
};

