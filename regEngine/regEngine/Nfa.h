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
	std::list<char> matchedChar;
	static char *regRead;

public:
	Nfa(char *reg);
	int match(char *file);


private:
	State *regex2nfa(char *reg, State *s);
	State *group(State *top);
	State *preDefine(State *top);
	Edge *newEdge(State * start, State * end, int type, int exclude);
	int step(State *current, char *c);
	void printMatched();
	void refresh();
};

#endif 

