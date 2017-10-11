#pragma once
#ifndef NFA_H

#include <iostream>
#include "State.h"
#include<list>
#include<queue>

using namespace std;

class Nfa 
{
public:
	Nfa(char *regex);
	//char match(char *file);
	static std::list<Edge *> edgeList;
	static std::list<State *> stateList;
	int match(char *file);
	static std::queue<char*> matchedChar;

private:
	State *Start;
	State *End;
	State *regex2nfa(char *regex, State *s);
	State *group(char *p, State *top);
	State *preDefine(char *p, State *top);
	Edge *newEdge(State * start, State * end, int type, int exclude);
	int step(State *current, char *c);
};


#endif NFA_H

