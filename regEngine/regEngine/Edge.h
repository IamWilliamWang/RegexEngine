#ifndef EDGE_H

#include <vector>
#include "State.h"
enum CHARTYPE{ LCASES=256, UCASES=257, NUM=258, EPSILON=259, ANY=260, WS=262 };
enum IFEXCLUDE{ EXCLUDED = true, NEXCLUDED = false};

class Edge
{
public:
	Edge(State *start, State *end, int type, IFEXCLUDE exclude = NEXCLUDED);
	State *start;
	State *end;
	CHARTYPE type;
	IFEXCLUDE exclude;

	bool match(char c);
	State *stepNext();
};

#endif // !EDGE_H

