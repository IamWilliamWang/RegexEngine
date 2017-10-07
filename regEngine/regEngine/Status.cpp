#include "Status.h"

Status::Status(bool FinalStatus, vector<Edge*> InEdges, vector<Edge*> OutEdges)
{
	this->FinalStatus = false;
	this->InEdges = InEdges;
	this->OutEdges = OutEdges;
}
