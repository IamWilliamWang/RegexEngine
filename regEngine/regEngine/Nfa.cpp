#include "Nfa.h"
#include <iostream>
#include <stack>
#include "State.h"
#include "Edge.h"

using namespace std;

Nfa::Nfa(char *regex)
{
	if ((this->Start = regex2nfa(regex)) != NULL) {
		cout << "NFA has built successfully!" << endl;
	} else {
		cout << "NFA built failed, please check if the regular expression is right!" << endl;
	}		
}

State *Nfa::regex2nfa(char *regex) 
{
	char *p;
	Edge *out;
	State *start, *s, *root;

	if (regex == NULL)
		return NULL;
	
	start = new State(false, nullptr, nullptr);
	stateStack->push(start);
	for (p = regex; *p; p++) {
		switch (*p) {
		default:
			out = new Edge(stateStack->top, nullptr, *p);
			s = new State(false, out, nullptr);
			patch(out, s);
			patch(stateStack->top, out);
			stateStack->push(s);
			edgeStack->push(out);
			break;
		case '.':	/* any */
			out = new Edge(stateStack->top, nullptr, *p);
			s = new State(false, out, nullptr);
			patch(out, s);
			patch(stateStack->top, out);
			stateStack->push(s);
			edgeStack->push(out);
			break;
		case '|':	/* alternate */
			p++;
			out = new Edge(root, nullptr, *p);
			s = new State(false, out, nullptr);
			patch(out, s);
			patch(root, out);
			stateStack->push(s);
			edgeStack->push(out);
			break;
		case '?':	/* zero or one */
			out = new Edge(edgeStack->top->start, stateStack->top, NULL);			
			patch(out, stateStack->top);
			patch(edgeStack->top->start, out);
			edgeStack->push(out);
			break;
		case '*':	/* zero or more */
			out = new Edge(edgeStack->top->start, edgeStack->top->start, edgeStack->top->character);
			edgeStack->top->character = NULL;
			patch(out, edgeStack->top->start);
			patch(edgeStack->top->start, out);
			edgeStack->push(out);
			break;
		case '+':	/* one or more */
			out = new Edge(stateStack->top, stateStack->top, edgeStack->top->character);			
			patch(out, stateStack->top);
			patch(stateStack->top, out);
			edgeStack->push(out);
			out = new Edge(stateStack->top, nullptr, NULL);
			s = new State(false, out, nullptr);
			patch(out, s);
			patch(stateStack->top, out);
			stateStack->push(s);
			edgeStack->push(out);
			break;
		case '[':
		}
	}
	return start;
}

int match(char *file)
{	
	ifstream in("file");
	while
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

void patch(Edge *e, State *s) {
	e->end = s;
	s->InEdges.push_back(e);
}

void patch(State *s, Edge *e) {
	s->OutEdges.push_back(e);
	e->start = s;
}