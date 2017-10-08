#pragma once
#ifndef STATE_H

#include<vector>
#include"Edge.h"
using namespace std;

class State
{
public:
	State(bool FinalStatus, Edge* InEdge,	Edge*OutEdge);
	bool FinalStatus;
	vector<Edge*> InEdges;
	vector<Edge*> OutEdges;
};


#endif STATE_H
