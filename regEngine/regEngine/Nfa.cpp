#include "Nfa.h"
#include <iostream>
#include <stack>
#include "State.h"
#include "Edge.h"

using namespace std;

Nfa::Nfa(char *regex)
{	
	Start = new State(false, nullptr, nullptr);
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
			out = new Edge(currentStart, nullptr, ANY);
			patch(currentStart, out);
			currentEnd = new State(false, out, nullptr);
			patch(out, currentEnd);			
			stateList->push_back(currentEnd);
			edgeList->push_back(out);
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
			edgeList->push_back(out);
			break;
		case '*':	// zero or more 
			if (currentEnd != currentStart + 1) //for case of group
			{
				out = newEdge(currentEnd, currentStart, EPSILON);
				edgeList->push_back(out);
				out = newEdge(currentStart, currentEnd, EPSILON);
				edgeList->push_back(out);
			}
			out = newEdge(currentStart, currentStart, edgeList->back->character);		
			edgeList->pop_back();
			stateList->remove(currentEnd);
			edgeList->push_back(out);
			break;
		case '+':	/* one or more */
			out = newEdge(currentEnd, currentStart, EPSILON);			
			edgeList->push_back(out);
			break;
		case 'ги':
			p++;
			currentEnd = regex2nfa(p, stateList->back);
			stateList->push_back(currentEnd);
			break;
		case ')':
			return currentEnd;
		case '[':
			p++;			
			if((currentEnd = group(p, stateList->back)) == nullptr) return nullptr;
			stateList->push_back(currentEnd);
			break;
		case '^':
			p++;
			out = new Edge(stateList->back, nullptr, *p, EXCLUDED);
			currentEnd = new State(false, out, nullptr);
			patch(out, currentEnd);
			patch(stateList->back, out);
			stateList->push_back(currentEnd);
			edgeList->push_back(out);
			break;
		case '\\':
			p++;
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
			out = new Edge(stateList->back, nullptr, *p);
			currentEnd = new State(false, out, nullptr);
			patch(out, currentEnd);
			patch(stateList->back, out);
			stateList->push_back(currentEnd);
			edgeList->push_back(out);
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
	State *s = new State(false, nullptr, nullptr);

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
			out = new Edge(stateList->back, nullptr, NUM, ifexclude);
			break;
		case 'z':
			out = new Edge(stateList->back, nullptr, LCASES, ifexclude);
			break;
		case 'Z':
			out = new Edge(stateList->back, nullptr, UCASES, ifexclude);
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;
		}		
		patch(out, s);
		patch(top, out);		
		edgeList->push_back(out);
	}
	return s;
}

State *Nfa::preDefine(char *p, State *top) {
	Edge *out;
	State *s = new State(false, nullptr, nullptr);
	for (p; *p != ']'; p++) {
		switch (*p) {
		case 'd':
			out = new Edge(stateList->back, nullptr, NUM);
			break;
		case 'D':
			out = new Edge(stateList->back, nullptr, NUM, EXCLUDED);
			break;
		case 's':
			out = new Edge(stateList->back, nullptr, WS);
			break;
		case 'S':
			out = new Edge(stateList->back, nullptr, WS, EXCLUDED);
			break;
		case 'w':
			out = new Edge(stateList->back, nullptr, NUM);
			Edge *out2 = new Edge(stateList->back, nullptr, UCASES);
			Edge *out3 = new Edge(stateList->back, nullptr, LCASES);
			patch(out2, s);
			patch(top, out2);
			patch(out3, s);
			patch(top, out3);
			edgeList->push_back(out2);
			edgeList->push_back(out3);
			break;
		case 'W':
			out = new Edge(stateList->back, nullptr, NUM, EXCLUDED);
			Edge *out2 = new Edge(stateList->back, nullptr, UCASES, EXCLUDED);
			Edge *out3 = new Edge(stateList->back, nullptr, LCASES, EXCLUDED);
			patch(out2, s);
			patch(top, out2);
			patch(out3, s);
			patch(top, out3);
			edgeList->push_back(out2);
			edgeList->push_back(out3);
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;
		}
		patch(out, s);
		patch(top, out);
		edgeList->push_back(out);
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
	return out;
}

State N
 
