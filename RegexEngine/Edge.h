#pragma once

#include "State.h"
#include <vector>

#ifndef EDGE_H
#define EDGE_H

enum { NEXCLUDED = false, EXCLUDED = true };
enum Type { LCASES=256, UCASES=257, NUM=258, EPSILON=259, ANY=260, WS=261, INPUTCHAR=262 };

class State;
class Edge
{
private:
	State *start;
	State *end;
	Type type;
	bool exclude;
	char inputChar;//当Type是INPUTCHAR时储存char，不是的时候储存'\0'
public:
	/*
	 *
	 */
	Edge(State *start, State *end, Type type, bool exclude = NEXCLUDED) :start(start), end(end), type(type), inputChar('\0'),exclude(exclude) {};
	Edge(State *start, State *end, char inputChar, bool exclude = NEXCLUDED) :start(start), end(end), type(Type::INPUTCHAR), inputChar(inputChar), exclude(exclude) {};
	/*
	 *
	 */
	bool match(char *p) {
		switch (type)
		{
		case LCASES:
			if ( *p >= 'a' && *p <= 'z') return !this->exclude;
			break;
		case UCASES:
			if ( *p >= 'A' && *p <= 'Z') return !this->exclude;
			break;
		case NUM:
			if (*p >= '0' && *p <= '9') return !this->exclude;
			break;
		case ANY:
			if (*p > -1 && *p < 128) return !this->exclude;
			break;
		case WS:
 			if ((*p == '\t') || (*p == '\n') || (*p == '\f')|| (*p == '\r') || (*p == '\x0B')) return !this->exclude;
			break;
		case INPUTCHAR:
			if (this->inputChar == *p)
				return !this->exclude;
		default:
			throw "Unexpected Type!";
		}
		return this->exclude;
	};	

	/* Getters and setters */
	void setStart(State* start)	{
		this->start = start;
	}
	State* getStart() {
		return this->start;
	}
	void setEnd(State* end) {
		this->end = end;
	}
	State* getEnd() {
		return this->end;
	}
	void setType(Type type) {
		this->type = type;
	}
	Type getType() {
		return this->type;
	}
	void setExclude(bool exclude) {
		this->exclude = exclude;
	}
	bool getExclude() {
		return this->exclude;
	}
	char getInputChar() {
		return this->inputChar;
	}
};

#endif 

