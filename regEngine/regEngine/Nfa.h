#ifndef NFA_H
#define MAXNFA 1024;
#include <iostream>
#include "State.h"

using namespace std;

class Nfa 
{
public:
	Nfa(char *regex);
	char match(char *file);
	static stack<Edge*> edgeStack[500];
	static stack<State*> stateStack[500];

private:
	State *Start;
	State *regex2nfa(char *regex);
};


#endif NFA_H

