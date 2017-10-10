#include "Nfa.h"
#include <iostream>
#include <stack>
#include "State.h"
#include "Edge.h"

using namespace std;

Nfa::Nfa(char *regex)
{	
	Start = new State();
	stateList->push_back(Start);
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
	State *currentEnd, *currentStart;

	if (regex == NULL)
		return NULL;
		
	for (p = regex; *p; p++) {
		currentEnd = stateList->back;
		switch (*p) {
		case '.':	/* any */
			currentStart = currentEnd;
			currentEnd = new State();
			out = newEdge(currentStart, currentEnd, ANY);		
			stateList->push_back(currentEnd);
			break;
		case '|':	// alternate 
			p++;
			currentStart = Start;
			State *alternate = regex2nfa(p, Start);
			currentEnd->merge(alternate);
			stateList->remove(alternate);
			break;
		case '?':	// zero or one 
			out = newEdge(currentStart, currentEnd, EPSILON);
			break;
		case '*':	// zero or more 
			if (currentEnd != currentStart + 1) //for case of group
			{
				out = newEdge(currentEnd, currentStart, EPSILON);
				out = newEdge(currentStart, currentEnd, EPSILON);
			}
			edgeList->pop_back();
			out = newEdge(currentStart, currentStart, edgeList->back->character);
			stateList->pop_back();
			currentEnd = stateList->back;
			break;
		case '+':	/* one or more */
			out = newEdge(currentEnd, currentStart, EPSILON);			
			edgeList->push_back(out);
			break;
		case 'ги':
			p++;
			currentStart = Start;
			currentEnd = regex2nfa(p, stateList->back);
			break;
		case ')':
			return currentEnd;
		case '[':
			p++;		
			currentStart = Start;
			if((currentEnd = group(p, stateList->back)) == nullptr) return nullptr;
			stateList->push_back(currentEnd);
			break;
		case '^':
			p++;
			currentEnd = new State();
			out = newEdge(stateList->back, currentEnd, *p, EXCLUDED);
			stateList->push_back(currentEnd);
			break;
		case '\\':
			p++;
			currentStart = Start;
			if ((currentEnd = preDefine(p, stateList->back)) == nullptr) return nullptr;
			stateList->push_back(currentEnd);
			break;
		case '\t':
		case '\n':
		case '\f':
		case '\r':
		case '\x0B':
			break;
		default:
			currentStart = currentEnd;
			currentEnd = new State();
			out = newEdge(stateList->back, currentEnd, *p);
			stateList->push_back(currentEnd);
			break;
		}
	}
	return currentEnd;
}

void patch(Edge *e, State *s) {
	e->end = s;
	s->InEdges.push_back(e);
}

void patch(State *s, Edge *e) {
	s->OutEdges.push_back(e);
	e->start = s;
}

State *Nfa::group(char *p, State *top) {
	Edge *out;
	State *s = new State();

	bool ifexclude = NEXCLUDED;
	if (*p == '^') ifexclude = EXCLUDED;

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
			out = newEdge(top, s, NUM, ifexclude);
			break;
		case 'z':
			out = newEdge(top, s, LCASES, ifexclude);
			break;
		case 'Z':
			out = newEdge(top, s, UCASES, ifexclude);
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;
		}		
	}
	return s;
}

State *Nfa::preDefine(char *p, State *top) {
	Edge *out;
	State *s = new State();
	for (p; *p != ']'; p++) {
		switch (*p) {
		case 'd':
			out = newEdge(top, s, NUM);
			break;
		case 'D':
			out = newEdge(top, s, NUM, EXCLUDED);
			break;
		case 's':
			out = newEdge(top, s, WS);
			break;
		case 'S':
			out = newEdge(top, s, WS, EXCLUDED);
			break;
		case 'w':
			out = newEdge(top, s, NUM);
			Edge *out2 = newEdge(top, s, UCASES);
			Edge *out3 = newEdge(top, s, LCASES);
			break;
		case 'W':
			out = newEdge(top, s, NUM, EXCLUDED);
			Edge *out2 = newEdge(top, s, UCASES, EXCLUDED);
			Edge *out3 = newEdge(top, s, LCASES, EXCLUDED);
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;
		}		
	}
	return s;
}

/**int match(char *file)
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
		t = clist; clist = nlist; nlist = t;	// swap clist, nlist
	}
	return ismatch(clist);
}
*/

Edge *Nfa::newEdge(State * start, State * end, int type, bool exclude = NEXCLUDED) {
	Edge *out = new Edge(start, end, type, exclude);
	patch(out, end);
	patch(start, out);
	edgeList->push_back(out);
	return out;
}


 
