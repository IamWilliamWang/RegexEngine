#include "State.h"

State::State(bool FinalStatus, Edge* InEdge, Edge* OutEdge)
{
	this->FinalStatus = false;
	this->InEdges.push_back(InEdge);
	this->OutEdges.push_back(OutEdge);
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