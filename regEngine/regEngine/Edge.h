#pragma once

#include "State.h"
#include <vector>

#ifndef EDGE_H
#define EDGE_H

enum { NEXCLUDED = false, EXCLUDED = true };
enum { LCASES=256, UCASES=257, NUM=258, EPSILON=259, ANY=260, WS=261 };

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
			break;
		case UCASES:
			if ( *p > 64 && *p < 91) return !this->exclude;
			break;
		case NUM:
			if (*p > 47 && *p < 58) return !this->exclude;
			break;
		case ANY:
			if (*p > -1 && *p < 128) return !this->exclude;
			break;
		case WS:
 			if ((*p == '\t') || (*p == '\n') || (*p == '\f')|| (*p == '\r') || (*p == '\x0B')) return !this->exclude;
			break;
		default:
			if (type == *p) return !this->exclude;
		}
		return this->exclude;
	};	
};

#endif 

