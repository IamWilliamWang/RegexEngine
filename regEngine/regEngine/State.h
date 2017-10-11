#pragma once
#ifndef STATE_H

#include<vector>
#include"Edge.h"
using namespace std;
enum { READY = 0, SUCCESS = 1, FAIL = -1};

class State
{
public:
	State();
	State(Edge *InEdge,	Edge *OutEdge, int status);
	int status;
	std::vector<Edge *> InEdges;
	std::vector<Edge *> OutEdges;
	void merge(State *s);
};


#endif STATE_H

