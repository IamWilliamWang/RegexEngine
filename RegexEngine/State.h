#pragma once
#ifndef STATE_H
#define STATE_H

#include<list>
#include"Edge.h"
#include "Nfa.h"
using namespace std;
enum Status{ READY = -1, SUCCESS = 1, FAIL = 0};

class State
{
private:
	Status status;
	std::list<Edge *> inEdges;
	std::list<Edge *> outEdges;

public:
	/*
	 * 
	 */
	State()
	{
		this->status = Status::READY;
	}
	/*
	 *
	 */
	State(Edge* inEdge, Edge* outEdge, Status status = Status::READY)
	{
		this->status = status;
		this->inEdges.emplace_back(inEdge);
		this->outEdges.emplace_back(outEdge);
	}
	/*
	 *
	 */
	void merge(State *s) {
		while (!s->inEdges.empty())

		{
			patch(s->inEdges.back(), this);
			s->inEdges.pop_back();
		}
		while (!s->outEdges.empty())
		{
			patch(this, s->inEdges.back());
			s->outEdges.pop_back();
		}
	}
	/*
	 *
	 */
	void patch(Edge *e, State *s)
	{
		e->setEnd(s);
		s->getInEdges().emplace_back(e);
	}
	/*
	 *
	 */
	void patch(State *s, Edge *e)
	{
		e->setStart(s);
		s->getOutEdges().emplace_back(e);
	}
	
	/* Getters and setters*/
	void setStatus(Status status) {
		this->status = status;
	}	
	Status getStatus() {
		return this->status;
	}	
	void setInEdges(std::list<Edge *> inEdges)	{
		this->inEdges = inEdges;
	}	
	std::list<Edge *> getInEdges()	{
		return this->inEdges;
	}	
	void setOutEdges(std::list<Edge *> outEdges)	{
		this->outEdges = outEdges;
	}	
	std::list<Edge *> getOutEdges()	{
		return this->outEdges;
	}
};
#endif

