#pragma once
#ifndef NFA_H
#define NFA_H

#include <iostream>
#include "State.h"
#include<list>
#include<queue>

using namespace std;

class Nfa 
{
public:
	State *Start;
	State *End;
	std::list<Edge *> edgeList;
	std::list<State *> stateList;	

public:
	Nfa(char *regex);
	//char match(char *file);
	int match(char *file);
	std::queue<char*> matchedChar;

private:
	State *regex2nfa(char *regex, State *s);
	State *group(char *p, State *top);
	State *preDefine(char *p, State *top);
	Edge *newEdge(State * start, State * end, int type, int exclude);
	int step(State *current, char *c);
};

#endif 

