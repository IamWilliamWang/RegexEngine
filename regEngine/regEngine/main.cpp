#include <iostream>
#include "Nfa.h"

class Nfa;

int main(int argc, char **argv)
{
	if (argc < 3) {
		cout << "usage: regEngine regexp string..." << endl;
		return 1;
	}

	Nfa *nfa = new Nfa(argv[1]);

	for (int i = 2; i < argc; i++)
		if (nfa->match(argv[i]) == SUCCESS)
			cout << "Found Matched strings!" << endl;

	return 0;
}

