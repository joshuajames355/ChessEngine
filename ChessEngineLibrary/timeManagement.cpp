#include "timeManagement.h"



timeManagement::timeManagement()
{
}

timeManagement::timeManagement(long int wtime,long int btime, colours colour)
{
	//Assume their is always 30 moves left.
	const int movesLeft = 30;

	switch (colour)
	{
	case white:
		targetTime = wtime / movesLeft;
		break;
	case black:
		targetTime = btime / movesLeft;
		break;
	}

	startTime = std::chrono::high_resolution_clock::now();
	searchMode = timed;
}

timeManagement::timeManagement(int depth)
{
	targetDepth = depth;
	searchMode = fixedDepth;
}

timeManagement::~timeManagement()
{
}

bool timeManagement::isMoreTime(int searchDepth)
{
	if (searchMode == timed)
	{
		//Their is enough time for another iteration if we have used less than half of the target time;
		std::chrono::duration<double, std::milli> time_span = std::chrono::high_resolution_clock::now() - startTime;

		//Divides by 10 as it is assuming each depth is 5 times harder than the previous.
		return time_span.count() < targetTime / 5;
	}
	else if (searchMode == fixedDepth)
	{
		return searchDepth < targetDepth;
	}
}
