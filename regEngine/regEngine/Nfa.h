#ifndef NFA_H
#define MAXNFA 1024;
#include <iostream>
#include "Status.h"

using namespace std;

class Nfa {
public:
	Nfa(char *regx);
	~Nfa();
	char match(char *file);
private:
	Status *Start;

};




#endif NFA_H

