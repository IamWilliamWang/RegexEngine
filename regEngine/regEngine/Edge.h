#ifndef EDGE_H

#include <vector>
#include "Status.h"

class Edge
{
public:
	Edge(Status *start, Status *end, char character);
	Status *start;
	Status *end;
	char character;

	bool match(char c);
	Status *stepNext();
};

#endif // !EDGE_H

