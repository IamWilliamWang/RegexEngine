#include "State.h"

State::State(Edge* InEdge, Edge*OutEdge, int status = READY)
{
	this->status = status;
	this->InEdges.push_back(InEdge);
	this->OutEdges.push_back(OutEdge);
}

State::State()
{
	this->status = READY;
}

void State::merge(State *s) {
	while (!s->InEdges.empty())
	{
		patch(s->InEdges.back(),this);
		s->InEdges.pop_back();
	}
	while (!s->OutEdges.empty())
	{
		patch(this, s->InEdges.back());
		s->OutEdges.pop_back();
	}
}

void State::patch(Edge *e, State *s)
{
	e->end = s;
	s->InEdges.push_back(e);
}

void State::patch(State *s, Edge *e)
{
	e->start = s;
	s->OutEdges.push_back(e);
}





