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
	static char *fileRead;

public:
	Nfa(char *reg);
	int match(char *str);

private:
	State *regex2nfa(char *reg, State *s);
	State *group(State *top);
	State *preDefine(State *top);
	void newEdge(State * start, State * end, int type, int exclude);
	int step(State *current);
	void printMatched();
	void refresh();
};

#endif 

