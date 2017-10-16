#pragma once
#ifndef STATE_H
#define STATE_H

#include<list>
#include"Edge.h"
#include "Nfa.h"

using namespace std;
enum { READY = -1, SUCCESS = 1, FAIL = 0};

class State
{
public:
	int status;
	std::list<Edge *> InEdges;
	std::list<Edge *> OutEdges;

public:
	State();
	State(Edge *InEdge, Edge *OutEdge, int status);
	void merge(State *s);
	void patch(Edge *e, State *s);
	void patch(State *s, Edge *e);
};

#endif

