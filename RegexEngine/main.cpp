#include <iostream>
#include "Nfa.h"

class Nfa;

int main()
{
	
	Nfa *nfa = new Nfa("a*(0|1)");
	cout << nfa->match("daa0");
	return 0;
}
