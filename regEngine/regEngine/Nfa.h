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
	char *regex;
	State *Start;
	State *End;
	std::list<Edge *> edgeList;
	std::list<State *> stateList;
	std::queue<char> matchedChar;

public:
	Nfa(char *reg);
	int match(char *file);


private:
	State *regex2nfa(char *regex, State *s);
	State *group(char *p, State *top);
	State *preDefine(char *p, State *top);
	Edge *newEdge(State * start, State * end, int type, int exclude);
	int step(State *current, char *c);
	void refresh();
};

#endif 

