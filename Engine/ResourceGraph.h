#ifndef _ResourceGraph_H_
#define _ResourceGraph_H_

#include "Resource.h"
#include <map>
#include "MathGeoLib\include\Math\MathAll.h"

enum linkType
{
	INPUT_LINK,
	OUTPUT_LINK
};

struct NodeLink
{
	NodeLink(linkType type, uint nodeID);

	void connect(uint nodeLinkID) { connectedNodeLink = nodeLinkID; }

	float2 nodePos = { 0,0 };

	uint ID = 0;
	uint nodeID = 0;
	uint connectedNodeLink = 0;

	linkType type = INPUT_LINK;

	bool linking = false;
};

struct Transition;
struct Node
{
	Node(const char* name, uint graphID, float2 pos, float2 size = { 150, 80 });
	~Node() {};

	NodeLink* addLink(linkType type, bool addToList = true, uint ID = 0);
	uint drawLinks() const;

	void connectLink(uint linkID);

	uint node_ID = 0;
	std::string name;
	float2 pos, gridPos, size = { 0.0f,0.0f };

	uint graphID = 0;
	uint animationID = 0;

	std::list<NodeLink*> links;
	int inputCount = 0;
	int outputCount = 0;

	std::list<Transition*> transitions;
};

struct Transition
{
	Transition(NodeLink* output, NodeLink* input, uint graphID);

	bool drawLine(bool selected, float2 offset);

	uint graphID = 0;

	Node* origin = nullptr;
	Node* destination = nullptr;

	NodeLink* output = nullptr;
	NodeLink* input = nullptr;

};



class ResourceGraph : public Resource
{
public:
	ResourceGraph(uint ID = 0);
	~ResourceGraph();

	Node* addNode(const char* name, float2 pos = { 20,20 });
	void pushLink(NodeLink* link);

	Node* getNode(uint ID);
	NodeLink* getLink(uint ID);

public:

	std::map<uint, Node*> nodes;
	std::map<uint, NodeLink*> links;
};

#endif // !_ResourceGraph_H_
