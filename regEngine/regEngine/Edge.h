#pragma once
#ifndef EDGE_H

#include <vector>
#include "State.h"

enum { NEXCLUDED = 0, EXCLUDED = 1 };
enum { LCASES=256, UCASES=257, NUM=258, EPSILON=259, ANY=260, WS=262 };

class Edge
{
public:
	Edge(State*s, State*e, int t, int ex = NEXCLUDED);
	State *start;
	State *end;
	int type;
	int exclude;
};

#endif // !EDGE_H

