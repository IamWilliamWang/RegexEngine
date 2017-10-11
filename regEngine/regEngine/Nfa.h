#ifndef NFA_H
#define MAXNFA 1024;
#include <iostream>
#include "State.h"
#include<list>

using namespace std;

class Nfa 
{
public:
	Nfa(char *regex);
	//char match(char *file);
	static list<Edge*> edgeList[500];
	static list<State*> stateList[500];
	char *match(char *file);

private:
	State *Start;
	State *End;
	State *regex2nfa(char *regex, State *s);
	State *group(char *p, State *top);
	State *preDefine(char *p, State *top);
	Edge * newEdge(State * start, State * end, int type, bool exclude = NEXCLUDED);
	Status step(State *current, char *c);
};


#endif NFA_H

