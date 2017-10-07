#include "Nfa.h"



Nfa::Nfa(char *regx)
{
	
}


Nfa::~Nfa()
{
}

char Nfa::match(char *file) 
{
	int i, c;
	List *clist, *nlist, *t;
	clist = startlist(start, &l1);
	nlist = &l2;
	for (; *s; s++) {
		c = *s & 0xFF;
		step(clist, c, nlist);
		t = clist; clist = nlist; nlist = t;	/* swap clist, nlist */
	}

	return ismatch(clist);

}