#include <iostream>
#include <fstream>
#include <stack>
#include "State.h"
#include "Edge.h"
#include "Nfa.h"

using namespace std;

Nfa::Nfa(char *regex)
{	
	Start = new State();
	stateList.push_back(Start);
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
	State *alternate;

	if (regex == NULL)
		return NULL;
		
	for (p = regex; *p; p++) {
		currentEnd = stateList.back();
		switch (*p) {
		case '.':	/* any */
			currentStart = currentEnd;
			currentEnd = new State();
			out = newEdge(currentStart, currentEnd, ANY, NEXCLUDED);		
			stateList.push_back(currentEnd);
			break;
		case '|':	// alternate 
			p++;
			currentStart = Start;
			alternate= regex2nfa(p, Start);
			currentEnd->merge(alternate);
			stateList.remove(alternate);
			break;
		case '?':	// zero or one 
			out = newEdge(currentStart, currentEnd, EPSILON, NEXCLUDED);
			break;
		case '*':	// zero or more 
			if (currentEnd != currentStart + 1) //for case of group
			{
				out = newEdge(currentEnd, currentStart, EPSILON, NEXCLUDED);
				out = newEdge(currentStart, currentEnd, EPSILON, NEXCLUDED);
			}
			edgeList.pop_back();
			currentEnd = currentStart;
			out = edgeList.back();
			out = newEdge(currentStart, currentEnd, out->type, NEXCLUDED);
			stateList.pop_back();
			currentEnd = stateList.back();
			break;
		case '+':	/* one or more */
			out = newEdge(currentEnd, currentStart, EPSILON, NEXCLUDED);
			edgeList.push_back(out);
			break;
		case 'ги':
			p++;
			currentStart = Start;
			currentEnd = regex2nfa(p, stateList.back());
			break;
		case ')':
			return currentEnd;
		case '[':
			p++;		
			currentStart = Start;
			if((currentEnd = group(p, stateList.back())) == nullptr) return nullptr;
			stateList.push_back(currentEnd);
			break;
		case '^':
			p++;
			currentStart = currentEnd;
			currentEnd = new State();
			out = newEdge(currentStart, currentEnd, *p, EXCLUDED);
			stateList.push_back(currentEnd);
			break;
		case '\\':
			p++;
			currentStart = Start;
			if ((currentEnd = preDefine(p, stateList.back())) == nullptr) return nullptr;
			stateList.push_back(currentEnd);
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
			out = newEdge(currentStart, currentEnd, *p, NEXCLUDED);
			stateList.push_back(currentEnd);
			break;
		}
	}
	return currentEnd;
}

void patch(Edge *e, State *s)
{
	e->end = s;
	s->InEdges.push_back(e);
}

void patch(State *s, Edge *e)
{
	s->OutEdges.push_back(e);
	e->start = s;
}

State *Nfa::group(char *p, State *top)
{
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

State *Nfa::preDefine(char *p, State *top)
{
	Edge *out, *out2, *out3;
	State *s = new State();
	for (p; *p != ']'; p++) {
		switch (*p) {
		case 'd':
			out = newEdge(top, s, NUM, NEXCLUDED);
			break;
		case 'D':
			out = newEdge(top, s, NUM, EXCLUDED);
			break;
		case 's':
			out = newEdge(top, s, WS, NEXCLUDED);
			break;
		case 'S':
			out = newEdge(top, s, WS, EXCLUDED);
			break;
		case 'w':
			out = newEdge(top, s, NUM, NEXCLUDED);
			out2 = newEdge(top, s, UCASES, NEXCLUDED);
			out3 = newEdge(top, s, LCASES, NEXCLUDED);
			break;
		case 'W':
			out = newEdge(top, s, NUM, EXCLUDED);
			out2 = newEdge(top, s, UCASES, EXCLUDED);
			out3 = newEdge(top, s, LCASES, EXCLUDED);
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;
		}		
	}
	return s;
}

Edge *Nfa::newEdge(State * start, State * end, int type, int exclude = NEXCLUDED)
{
	Edge *out = new Edge(start, end, type, exclude);
	patch(out, end);
	patch(start, out);
	edgeList.push_back(out);
	return out;
}

int Nfa::match(char *file)
{
	/*FILE *fp;
	if (!(fp = fopen(file, "r"))){
		cout << "File: " << *file << " failed, try again." << endl;
	}
	*/
	State *current = this->Start;
	current->status = SUCCESS;
	ifstream readByChar;
	char c[1024];
	readByChar.open(file, ios::in);
	if (!readByChar.is_open()) {
		cout << "Failed to open the file: " << file << endl;
		return FAIL;
	}
	while (!readByChar.eof())
	{	
		readByChar >> c;
		while ((step(current, c) == FAIL))
		{
			if (*c != EOF) {
				readByChar >> c;
				continue;
			}
			break;
		}
		if (this->End->status == FAIL)
		{
			continue;
		}
		while (!matchedChar.empty())
		{
			cout << matchedChar.front();
			matchedChar.pop();
		}
		cout << endl;
	}
	readByChar.close();
	return SUCCESS;
}
 
int Nfa::step(State *current,char *c)
{
	vector<Edge*> temp = current->OutEdges;
	Edge *currentEdge;
	char *ch = c;
	while (!temp.empty())
	{	
		currentEdge = temp.back();
		if (currentEdge->match(ch)) {
			currentEdge->end->status = SUCCESS;
			matchedChar.push(ch);
			return step(currentEdge->end, ++ch);
		}
		temp.pop_back();
	}
	matchedChar.push("\n");
	return FAIL;
}
