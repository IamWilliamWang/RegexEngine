#include "State.h"

State::State(bool FinalStatus, Edge* InEdge, Edge* OutEdge)
{
	this->FinalStatus = false;
	this->InEdges.push_back(InEdge);
	this->OutEdges.push_back(OutEdge);
}
