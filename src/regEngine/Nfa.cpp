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
	Edge *out;
	State *currentEnd, *currentStart;
	State *alternate;

	if (regex == NULL)
		return NULL;
		
	currentEnd = start;
	for (regRead = reg; *regRead; regRead++) {
		switch (*regRead) {
		case '.':	/* any */
			currentStart = currentEnd;
			currentEnd = new State();
			out = newEdge(currentStart, currentEnd, ANY, NEXCLUDED);		
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
			out = newEdge(currentStart, currentEnd, EPSILON, NEXCLUDED);
			break;
		case '*':	// zero or more 
			/*if (currentEnd != currentStart + 1) //for case of group
			{
				out = newEdge(currentEnd, currentStart, EPSILON, NEXCLUDED);
				out = newEdge(currentStart, currentEnd, EPSILON, NEXCLUDED);
			}*/
			/*edgeList.pop_back();
			currentEnd = currentStart;
			out = edgeList.back();
			out = newEdge(currentStart, currentEnd, out->type, NEXCLUDED);
			stateList.pop_back();
			currentEnd = stateList.back();*/
			alternate = currentEnd;
			currentStart->merge(alternate);
			stateList.remove(alternate);
			currentEnd = currentStart;
			break;
		case '+':	/* one or more */
			out = newEdge(currentEnd, currentEnd, edgeList.back()->type, NEXCLUDED);
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
			stateList.push_back(currentEnd);
			break;	
		case '^':
			regRead++;
			currentStart = currentEnd;
			currentEnd = new State();
			out = newEdge(currentStart, currentEnd, *regRead, EXCLUDED);
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
			out = newEdge(currentStart, currentEnd, *regRead, NEXCLUDED);
			stateList.push_back(currentEnd);
			break;
		}
	}
	return currentEnd;
}

State *Nfa::group(State *top)
{
	Edge *out;
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

State *Nfa::preDefine(State *top)
{
	Edge *out, *out2, *out3;
	State *s = new State();
	for (regRead; *regRead != ']'; regRead++) {
		switch (*regRead) {
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
		refresh();
		matchedChar.clear();
	}	
	return SUCCESS;
}
 
int Nfa::step(State *current)
{
	vector<Edge*> temp = current->OutEdges;
	Edge *currentEdge;

	if (End->status == SUCCESS) 
		return SUCCESS;

	while (!temp.empty())
	{	
		currentEdge = temp.back();
		if (currentEdge->match(fileRead))
		{
			currentEdge->end->status = SUCCESS;
			matchedChar.push_back(*fileRead);
			++fileRead;
			if (step(currentEdge->end))
			{
				return SUCCESS;
			}
			else
			{	
				--fileRead;
				matchedChar.pop_back();
			}
		}
		if (currentEdge->type == EPSILON && step(currentEdge->end))
			return SUCCESS;

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

