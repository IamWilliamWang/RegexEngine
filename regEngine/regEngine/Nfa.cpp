#include <iostream>
#include <fstream>
#include <stack>
#include "State.h"
#include "Edge.h"
#include "Nfa.h"
#define MAX_SIZE 512

using namespace std;

Nfa::Nfa(char *reg)
{	
	this->regex = reg;
	Start = new State();
	stateList.push_back(Start);
	if ((End = regex2nfa(regex, Start)) != nullptr) {
		cout << "NFA has built successfully!" << endl;
	} else {
		cout << "NFA built failed, please check if the regular expression is right!" << endl;
	}		
}

State *Nfa::regex2nfa(char *reg, State *start)
{	
	static char *p;
	Edge *out;
	State *currentEnd, *currentStart;
	State *alternate;

	if (regex == NULL)
		return NULL;
		
	currentEnd = start;
	for (p = reg; *p; p++) {
		switch (*p) {
		case '.':	/* any */
			currentStart = currentEnd;
			currentEnd = new State();
			out = newEdge(currentStart, currentEnd, ANY, NEXCLUDED);		
			stateList.push_back(currentEnd);
			break;
		case '|':	// alternate 
			p++;
			currentStart = start;
			alternate= regex2nfa(p, start);
			currentEnd->merge(alternate);
			stateList.remove(alternate);
			p--;
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
		case '��':
			p++;
			currentStart = start;
			currentEnd = regex2nfa(p, currentEnd);
			break;
		case ')':
			return currentEnd;
		case '[':
			p++;		
			currentStart = start;
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
			currentStart = start;
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

State *Nfa::group(char *p, State *top)
{
	Edge *out;
	State *s = new State();

	bool ifexclude = NEXCLUDED;
	if (*p == '^') {
		p++;
		ifexclude = EXCLUDED;
	}

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
	end->patch(out, end);
	start->patch(start, out);
	edgeList.push_back(out);
	return out;
}

int Nfa::match(char *file)
{
	
	FILE *fp;
	if (!(fp = fopen(file, "r"))) 
	{
		cout << "File: " << *file << " open failed, try again." << endl;
		return NULL;
	}
	char result[MAX_SIZE] = { '\0' };
	int cnt = 0;
	for (; !feof(fp); cnt++) {
		char c = fgetc(fp);
		switch (c) {
		case '\t':
		case '\n':
		case '\f':
		case '\r':
		case '\x0B':
			cnt--;
			break;
		default:
			result[cnt] = c;
			break;
		}	
	}
	fclose(fp);

	State *current = this->Start;
	current->status = SUCCESS;
	char *p = result;
	
	while (*p != EOF)
	{
		if ((step(current, p) == FAIL))
		{
			p++;
			continue;
		}
		cout << "Matced characters: ";
		while (!matchedChar.empty())
		{
			cout << matchedChar.front();
			matchedChar.pop();
		}
		cout << endl;
		refresh();
		p++;
	}	
	return SUCCESS;
}
 
int Nfa::step(State *current,char *c)
{
	vector<Edge*> temp = current->OutEdges;
	Edge *currentEdge;

	if (End->status == SUCCESS) 
		return SUCCESS;

	while (!temp.empty())
	{	
		currentEdge = temp.back();
		if (currentEdge->match(c)) 
		{
			currentEdge->end->status = SUCCESS;
			matchedChar.push(*c);
			return step(currentEdge->end, ++c);
		}
		temp.pop_back();
	}
	
	return FAIL;
}

void Nfa::refresh() {  
	list<State*>::iterator itor;   
	itor = stateList.begin();
	State* current = *itor;
	itor++;
	while (itor != stateList.end())
	{
		(*itor)->status = FAIL;
		itor++;
	}

}

