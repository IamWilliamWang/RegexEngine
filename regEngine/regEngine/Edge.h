#ifndef EDGE_H

#include <vector>
#include "State.h"
enum { LCASES=256, UCASES=257, NUM=258, EPSILON=259, ANY=260, WS=262 };
enum { EXCLUDED = true, NEXCLUDED = false};

class Edge
{
public:
	Edge(State *start, State *end, int type, bool exclude = NEXCLUDED);
	State *start;
	State *end;
	int type;
	bool exclude;

	bool match(char c);
	State *stepNext();
};

#endif // !EDGE_H

