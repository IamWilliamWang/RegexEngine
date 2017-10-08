#ifndef EDGE_H

#include <vector>
#include "State.h"
enum { LCASES=256, UCASES=257, NUM=258, EPSILON=259, ANY=260 };

class Edge
{
public:
	Edge(State *start, State *end, int type);
	State *start;
	State *end;
	int type;

	bool match(char c);
	State *stepNext();
};

#endif // !EDGE_H

