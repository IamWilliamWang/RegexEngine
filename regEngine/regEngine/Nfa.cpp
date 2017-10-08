#include "Nfa.h"
#include <iostream>
#include <stack>
#include "State.h"
#include "Edge.h"

using namespace std;

Nfa::Nfa(char *regex)
{	
	Start = new State(false, nullptr, nullptr);
	stateStack->push(Start);
	if ((End = regex2nfa(regex, Start)) != nullptr) {
		cout << "NFA has built successfully!" << endl;
	} else {
		cout << "NFA built failed, please check if the regular expression is right!" << endl;
	}		
}

State *Nfa::regex2nfa(char *regex, State *Start) 
{
	char *p;
	Edge *out;
	State *s;

	if (regex == NULL)
		return NULL;
		
	for (p = regex; *p; p++) {
		switch (*p) {
		case '.':	/* any */
			out = new Edge(stateStack->top, nullptr, ANY);
			s = new State(false, out, nullptr);
			patch(out, s);
			patch(stateStack->top, out);
			stateStack->push(s);
			edgeStack->push(out);
			break;
		case '|':	/* alternate */
			p++;
			State *current = stateStack->top;
			out = new Edge(Start, nullptr, EPSILON);
			s = new State(false, out, nullptr);
			patch(out, s);
			patch(Start, out);
			stateStack->push(s);
			edgeStack->push(out);
			s = regex2nfa(p, s);
			out = new Edge(current, s, EPSILON);
			patch(out, s);
			patch(current, out);
			break;
		case '?':	/* zero or one */
			out = new Edge(edgeStack->top->start, stateStack->top, EPSILON);
			patch(out, stateStack->top);
			patch(edgeStack->top->start, out);
			edgeStack->push(out);
			break;
		case '*':	/* zero or more */
			out = new Edge(edgeStack->top->start, edgeStack->top->start, edgeStack->top->character);
			edgeStack->top->character = EPSILON;
			patch(out, edgeStack->top->start);
			patch(edgeStack->top->start, out);
			edgeStack->push(out);
			break;
		case '+':	/* one or more */
			out = new Edge(stateStack->top, stateStack->top, edgeStack->top->character);			
			patch(out, stateStack->top);
			patch(stateStack->top, out);
			edgeStack->push(out);
			out = new Edge(stateStack->top, nullptr, EPSILON);
			s = new State(false, out, nullptr);
			patch(out, s);
			patch(stateStack->top, out);
			stateStack->push(s);
			edgeStack->push(out);
			break;
		case 'ги':
			p++;
			s = regex2nfa(p, stateStack->top);
			stateStack->push(s);
			break;
		case ')':
			return s;
		case '[':
			p++;			
			if((s = group(p)) == nullptr) return nullptr;
			break;
		default:
			out = new Edge(stateStack->top, nullptr, *p);
			s = new State(false, out, nullptr);
			patch(out, s);
			patch(stateStack->top, out);
			stateStack->push(s);
			edgeStack->push(out);
			break;
		}
	}
	return s;
}

void patch(Edge *e, State *s) {
	e->end = s;
	s->InEdges.push_back(e);
}

void patch(State *s, Edge *e) {
	s->OutEdges.push_back(e);
	e->start = s;
}

State *Nfa::group(char *p) {
	Edge *out;
	for (p; *p!=']'; p++) {
		switch (*p) {
		case '0':
		case 'a':
		case 'A':
			p++;
			if (*p != '-') {
				cout << "NFA built failed, please check if the regular expression is right!" << endl;
				return	nullptr;
			}
			break;
		case '9':
			out = new Edge(stateStack->top, nullptr, NUM);
			break;
		case 'z':
			out = new Edge(stateStack->top, nullptr, LCASES);
			break;
		case 'Z':
			out = new Edge(stateStack->top, nullptr, UCASES);
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;
		}
		State *s = new State(false, out, nullptr);
		patch(out, s);
		patch(stateStack->top, out);
		stateStack->push(s);
		edgeStack->push(out);
	}
	return stateStack->top;
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
