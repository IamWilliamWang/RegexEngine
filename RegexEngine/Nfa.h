#pragma once
#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <fstream>
#include <stack>
#include "State.h"
#include "Edge.h"
#include "Nfa.h"
#include<list>
#include<queue>
#define MAX_SIZE 512
using namespace std;

char *regRead;
char *fileRead;
class Nfa 
{
public:
	/*
	 *
	 */
	Nfa(char *reg)
	{
		this->regex = reg;
		start = new State();
		stateList.emplace_back(start);
		if ((end = regex2nfa(regex, start)) != nullptr) {
			cout << "NFA has built successfully!" << endl;
		}
		else {
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
		}
	}
	/*
	 *
	 */
	int match(char *content)
	{
		bool everMatched = false;
		this->start->setStatus(Status::SUCCESS);
		fileRead = content;

		while (*fileRead != '\0')
		{
			if (step(this->start) == Status::FAIL)
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

private:
	/*
	 *
	 */
	State *regex2nfa(char *reg, State *start)
	{
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
				newEdge(currentStart, currentEnd, Type::ANY, NEXCLUDED);
				stateList.emplace_back(currentEnd);
				break;
			case '|':	// alternate 
				regRead++;
				currentStart = start;
				alternate = regex2nfa(regRead, start);
				currentEnd->merge(alternate);
				stateList.remove(alternate);
				regRead--;
				break;
			case '?':	// zero or one 
				newEdge(currentStart, currentEnd, Type::EPSILON, NEXCLUDED);
				break;
			case '*':	// zero or more 
				alternate = currentEnd;
				currentStart->merge(alternate);
				stateList.remove(alternate);
				currentEnd = currentStart;
				break;
			case '+':	/* one or more */
				for (Edge* edge : currentStart->getOutEdges())
					if (edge->getType() == Type::INPUTCHAR)
						newEdge(currentEnd, edge->getEnd(), edge->getInputChar(), edge->getExclude());
					else
						newEdge(currentEnd, edge->getEnd(), edge->getType(), edge->getExclude());
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
				if ((currentEnd = group(currentEnd)) == nullptr) return nullptr;
				break;
			case '^':
				regRead++;
				currentStart = currentEnd;
				currentEnd = new State();
				newEdge(currentStart, currentEnd, *regRead, EXCLUDED);
				stateList.emplace_back(currentEnd);
				break;
			case '\\':
				regRead++;
				currentStart = start;
				if ((currentEnd = preDefine(currentEnd)) == nullptr) return nullptr;
				stateList.emplace_back(currentEnd);
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
				stateList.emplace_back(currentEnd);
				break;
			}
		}
		return currentEnd;
	}
	/*
	 *
	 */
	State *group(State *top)
	{
		State *s = new State();
		bool ifexclude = NEXCLUDED;
		if (*regRead == '^') {
			regRead++;
			ifexclude = EXCLUDED;
		}
		for (regRead; *regRead != ']'; regRead++) {
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
				newEdge(top, s, Type::NUM, ifexclude);
				break;
			case 'z':
				newEdge(top, s, Type::LCASES, ifexclude);
				break;
			case 'Z':
				newEdge(top, s, Type::UCASES, ifexclude);
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
		stateList.emplace_back(s);
		return s;
	}
	/*
	 *
	 */
	State *preDefine(State *top)
	{
		State *s = new State();
		switch (*regRead) {
		case 'd':
			newEdge(top, s, Type::NUM, NEXCLUDED);
			break;
		case 'D':
			newEdge(top, s, Type::NUM, EXCLUDED);
			break;
		case 's':
			newEdge(top, s, Type::WS, NEXCLUDED);
			break;
		case 'S':
			newEdge(top, s, Type::WS, EXCLUDED);
			break;
		case 'w':
			newEdge(top, s, Type::NUM, NEXCLUDED);
			newEdge(top, s, Type::UCASES, NEXCLUDED);
			newEdge(top, s, Type::LCASES, NEXCLUDED);
			break;
		case 'W':
			newEdge(top, s, Type::NUM, EXCLUDED);
			newEdge(top, s, Type::UCASES, EXCLUDED);
			newEdge(top, s, Type::LCASES, EXCLUDED);
			break;
		default:
			cout << "NFA built failed, please check if the regular expression is right!" << endl;
			return	nullptr;
		}
		return s;
	}
	/*
	 *
	 */
	void newEdge(State * start, State * end, Type type, bool exclude = NEXCLUDED)
	{
		Edge *out = new Edge(start, end, type, exclude);
		end->patch(out, end);
		start->patch(start, out);
		edgeList.emplace_back(out);
	}
	void newEdge(State* start, State* end, char inputChar, bool exclude = NEXCLUDED)
	{
		Edge *out;
		if(inputChar== 256)
			out = new Edge(start, end, LCASES, exclude);
		else if (inputChar == 257)
			out = new Edge(start, end, UCASES, exclude);
		else if (inputChar == 258)
			out = new Edge(start, end, NUM, exclude);
		else if (inputChar == 259)
			out = new Edge(start, end, EPSILON, exclude);
		else if (inputChar == 260)
			out = new Edge(start, end, ANY, exclude);
		else if (inputChar == 261)
			out = new Edge(start, end, WS, exclude);
		else
			out = new Edge(start, end, inputChar, exclude);
		end->patch(out, end);
		start->patch(start, out);
		edgeList.emplace_back(out);
	}
	/*
	 *
	 */
	int step(State *current)
	{
		if (end->getStatus() == SUCCESS)
			return SUCCESS;

		for (Edge* edge: current->getOutEdges())
		{
			if (edge->match(fileRead))
			{
				edge->getEnd()->setStatus(Status::SUCCESS);
				matchedChar.emplace_back(*fileRead);
				++fileRead;
				if (step(edge->getEnd()))
					return SUCCESS;

				--fileRead;
				matchedChar.pop_back();
			}
			if (edge->getType() == Type::EPSILON && step(edge->getEnd()))
				return SUCCESS;
		}
		return FAIL;
	}
	/*
	 *
	 */
	void printMatched() {
		list<char>::iterator itor;
		itor = matchedChar.begin();

		cout << "Matced characters: ";
		while (itor != matchedChar.end())
		{
			cout << *itor++;
		}
		cout << endl;
	}
	/*
	 *
	 */
	void refresh() {
		list<State*>::iterator itor;
		itor = stateList.begin();
		State* current = *itor;
		itor++;
		while (itor != stateList.end())
		{
			(*itor)->setStatus(Status::FAIL);
			itor++;
		}
	}
	
	char *regex;
	State *start;
	State *end;
	std::list<Edge *> edgeList;
	std::list<State *> stateList;
	std::list<char> matchedChar;
	/* Getters and setters*/
	void setRegex(char* regex) {
		this->regex = regex;
	}
	char* getRegex() {
		return this->regex;
	}
	void setStart(State* start)	{
		this->start = start;
	}
	State* getStart() {
		return this->start;
	}
	void setEnd(State* end)	{
		this->end = end;
	}
	State* getEnd()	{
		return this->end;
	}
	void setEdgeList(std::list<Edge *> edgeList) {
		this->edgeList = edgeList;
	}
	std::list<Edge *>getEdgeList()	{
		return this->edgeList;
	}
	void setStateList(std::list<State *> stateList)	{
		this->stateList = stateList;
	}
	std::list<State *>getStateList() {
		return this->stateList;
	}
	void setMatchedChar(std::list<char> matchedChar) {
		this->matchedChar = matchedChar;
	}
	std::list<char> getMatchedChar() {
		return this->matchedChar;
	}
};

#endif 

