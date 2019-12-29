#include "Application.h"

#include "ResourceGraph.h"

ResourceGraph::ResourceGraph(uint ID) :Resource(Resource::RESOURCE_TYPE::RESOURCE_GRAPH, ID)
{
}

ResourceGraph::~ResourceGraph()
{
}

Node* ResourceGraph::addNode(const char * name, float2 pos)
{
	return nullptr;
}

void ResourceGraph::pushLink(NodeLink * link)
{
}

Node* ResourceGraph::getNode(uint ID)
{
	return nullptr;
}

NodeLink* ResourceGraph::getLink(uint ID)
{
	return nullptr;
}

NodeLink::NodeLink(linkType type, uint nodeID)
{
}

Node::Node(const char * name, uint graphID, float2 pos, float2 size)
{
}

NodeLink* Node::addLink(linkType type, bool addToList, uint ID)
{
	return nullptr;
}

uint Node::drawLinks() const
{
	return uint();
}

void Node::connectLink(uint linkID)
{
}

Transition::Transition(NodeLink * output, NodeLink * input, uint graphID)
{
}

bool Transition::drawLine(bool selected, float2 offset)
{
	return false;
}
