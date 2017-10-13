#pragma once
#ifndef STATE_H
#define STATE_H

#include<vector>
#include"Edge.h"
#include "Nfa.h"

using namespace std;
enum { READY = 0, SUCCESS = 1, FAIL = -1};

class State
{
public:
	int status;
	std::vector<Edge *> InEdges;
	std::vector<Edge *> OutEdges;

public:
	State();
	State(Edge *InEdge, Edge *OutEdge, int status);
	void merge(State *s);
	void patch(Edge *e, State *s);
	void patch(State *s, Edge *e);
};

#endif

