#pragma once

#include "State.h"
#include <vector>

#ifndef EDGE_H
#define EDGE_H

enum { NEXCLUDED = 0, EXCLUDED = 1 };
enum { LCASES=256, UCASES=257, NUM=258, EPSILON=259, ANY=260, WS=262 };

class State;
class Edge
{
public:
	State *start;
	State *end;
	int type;
	int exclude;

public:
	Edge(State *s, State *e, int t, int ex = NEXCLUDED) :start(s), end(e), type(t), exclude(ex) {};
	bool match(char *p) {
		switch (type)
		{
		case LCASES:
		case UCASES:
		case NUM:
		case EPSILON:
		case ANY:
		case WS:
		default:
			if (type == *p) return true;
		}
		

		return false;
	};
	
};

#endif 

