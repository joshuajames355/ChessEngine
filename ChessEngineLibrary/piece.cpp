#include "piece.h"

colours switchColour(colours colour)
{
	switch (colour)
	{
	case white:
		return black;
	case black:
		return white;
	}	
}

