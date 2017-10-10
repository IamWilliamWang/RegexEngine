#include "State.h"

State::State(Edge* InEdge, Edge*OutEdge, Status status = READY)
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
	while (s->InEdges.empty())
	{
		this->InEdges.push_back(s->InEdges.back());
		s->InEdges.pop_back();
	}
	while (s->OutEdges.empty())
	{
		this->OutEdges.push_back(s->OutEdges.back());
		s->OutEdges.pop_back();
	}
}