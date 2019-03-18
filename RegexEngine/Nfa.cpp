#include <iostream>
#include <fstream>
#include <stack>
#include "State.h"
#include "Edge.h"
#include "Nfa.h"
#define MAX_SIZE 512

using namespace std;
char *Nfa::regRead;
char *Nfa::fileRead;

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
	State *currentEnd, *currentStart;
	State *alternate;
	list<Edge *>::iterator itor;

	if (regex == NULL)
		return NULL;
		
	currentEnd = start;
	for (regRead = reg; *regRead; regRead++) {
		switch (*regRead) {
		case '.':	/* any */
			currentStart = currentEnd;
			currentEnd = new State();
			newEdge(currentStart, currentEnd, ANY, NEXCLUDED);		
			stateList.push_back(currentEnd);
			break;
		case '|':	// alternate 
			regRead++;
			currentStart = start;
			alternate= regex2nfa(regRead, start);
			currentEnd->merge(alternate);
			stateList.remove(alternate);
			regRead--;
			break;
		case '?':	// zero or one 
			newEdge(currentStart, currentEnd, EPSILON, NEXCLUDED);
			break;
		case '*':	// zero or more 
			alternate = currentEnd;
			currentStart->merge(alternate);
			stateList.remove(alternate);
			currentEnd = currentStart;
			break;
		case '+':	/* one or more */
			itor = currentStart->OutEdges.begin();
			for (;itor != currentStart->OutEdges.end();itor++)
				newEdge(currentEnd, (*itor)->end, (*itor)->type, (*itor)->exclude);
			break;
		case '(':
			regRead++;
			currentStart = currentEnd;
			currentEnd = regex2nfa(regRead, currentEnd);
			break;
		case ')':
			return currentEnd;
		case '[':
			regRead++;
			currentStart = currentEnd;
			if((currentEnd = group(currentEnd)) == nullptr) return nullptr;
			break;	
		case '^':
			regRead++;
			currentStart = currentEnd;
			currentEnd = new State();
			newEdge(currentStart, currentEnd, *regRead, EXCLUDED);
			stateList.push_back(currentEnd);
			break;
		case '\\':
			regRead++;
			currentStart = start;
			if ((currentEnd = preDefine(currentEnd)) == nullptr) return nullptr;
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
			newEdge(currentStart, currentEnd, *regRead, NEXCLUDED);
			stateList.push_back(currentEnd);
			break;
		}
	}
	return currentEnd;
}

State *Nfa::group(State *top)
{
	State *s = new State();
	bool ifexclude = NEXCLUDED;
	if (*regRead == '^') {
		regRead++;
		ifexclude = EXCLUDED;
	}
	for (regRead; *regRead !=']'; regRead++) {
		switch (*regRead) {
		case '0':
		case 'a':
		case 'A':
			regRead++;
			if (*regRead != '-') {
				cout << "NFA built failed, please check if the regular expression is right!" << endl;
				return	nullptr;
			}
			break;
		case '9':
			newEdge(top, s, NUM, ifexclude);
			break;
		case 'z':
			newEdge(top, s, LCASES, ifexclude);
			break;
		case 'Z':
			newEdge(top, s, UCASES, ifexclude);
			break;
		case '\\':
			regRead++;
			if ((s = preDefine(top)) == nullptr) return nullptr;
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;
		}		
	}
	stateList.push_back(s);
	return s;
}

State *Nfa::preDefine(State *top)
{
	State *s = new State();
	switch (*regRead) {
		case 'd':
			newEdge(top, s, NUM, NEXCLUDED);
			break;
		case 'D':
			newEdge(top, s, NUM, EXCLUDED);
			break;
		case 's':
			newEdge(top, s, WS, NEXCLUDED);
			break;
		case 'S':
			newEdge(top, s, WS, EXCLUDED);
			break;
		case 'w':
			newEdge(top, s, NUM, NEXCLUDED);
			newEdge(top, s, UCASES, NEXCLUDED);
			newEdge(top, s, LCASES, NEXCLUDED);
			break;
		case 'W':
			newEdge(top, s, NUM, EXCLUDED);
			newEdge(top, s, UCASES, EXCLUDED);
			newEdge(top, s, LCASES, EXCLUDED);
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;		
	}
	return s;
}

void Nfa::newEdge(State * start, State * end, int type, int exclude = NEXCLUDED)
{
	Edge *out = new Edge(start, end, type, exclude);
	end->patch(out, end);
	start->patch(start, out);
	edgeList.push_back(out);
}

int Nfa::match(char *file)
{
	bool everMatched = false;
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

	this->Start->status = SUCCESS;
	fileRead = result;
	
	while (*fileRead != EOF)
	{
		if (step(this->Start) == FAIL)
		{
			fileRead++;
			matchedChar.clear();
			refresh();
			continue;
		}
		printMatched();
		everMatched = true;
		refresh();
		matchedChar.clear();
	}	
	if (everMatched)
		return SUCCESS;

	return FAIL;
}
 
int Nfa::step(State *current)
{	
	list<Edge *>::iterator itor;
	itor = current->OutEdges.begin();

	if (End->status == SUCCESS) 
		return SUCCESS;

	for(;itor != current->OutEdges.end();itor++)
	{	
		if ((*itor)->match(fileRead))
		{
			(*itor)->end->status = SUCCESS;
			matchedChar.push_back(*fileRead);
			++fileRead;
			if (step((*itor)->end))
				return SUCCESS;

			--fileRead;
			matchedChar.pop_back();
		}
		if ((*itor)->type == EPSILON && step((*itor)->end))
			return SUCCESS;
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

void Nfa::printMatched() {
	list<char>::iterator itor;
	itor = matchedChar.begin();

	cout << "Matced characters: ";
	while (itor != matchedChar.end())
	{
		cout << *itor++;
	}
	cout << endl;
}

