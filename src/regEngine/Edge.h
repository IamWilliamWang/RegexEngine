#pragma once

#include "State.h"
#include <vector>

#ifndef EDGE_H
#define EDGE_H

enum { NEXCLUDED = false, EXCLUDED = true };
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
	Edge(State *s, State *e, int t, bool ex = NEXCLUDED) :start(s), end(e), type(t), exclude(ex) {};
	bool match(char *p) {
		switch (type)
		{
		case LCASES:
			if ( *p > 96 && *p < 123) return !this->exclude;
		case UCASES:
			if ( *p > 64 && *p < 91) return !this->exclude;
		case NUM:
			if (*p > 47 && *p < 58) return !this->exclude;
		case ANY:
			if (*p > -1 && *p < 128) return !this->exclude;
		case WS:
			if ((*p > 96 && *p < 123) || (*p > 64 && *p < 91) || (*p > 47 && *p < 58)) return !this->exclude;
		default:
			if (type == *p) return !this->exclude;
		}
		return this->exclude;
	};	
};

#endif 

