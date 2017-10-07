#pragma once
#ifndef STATE_H

#include<vector>
#include"Edge.h"
using namespace std;

class Status
{
public:
	Status(bool FinalStatus, vector<Edge*> InEdges,	vector<Edge*> OutEdges);
	bool FinalStatus;
	vector<Edge*> InEdges;
	vector<Edge*> OutEdges;
};


#endif STATE_H

