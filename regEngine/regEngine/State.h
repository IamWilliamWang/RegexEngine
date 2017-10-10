#pragma once
#ifndef STATE_H

#include<vector>
#include"Edge.h"
using namespace std;
enum Status { READY = 0, SUCCESS = 1, FAIL = -1};

class State
{
public:
	State();
	State(Edge* InEdge,	Edge*OutEdge, Status status = READY);
	Status status;
	vector<Edge*> InEdges;
	vector<Edge*> OutEdges;
	void merge(State *s);
};


#endif STATE_H

