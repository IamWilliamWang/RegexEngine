#include <iostream>
#include "Nfa.h"

class Nfa;

int main()
{
	
	Nfa *nfa = new Nfa("a*[01]");
	cout << nfa->match("Xxdsdsasdaaaaaaaaaaaaaaa0");
	return 0;
}
