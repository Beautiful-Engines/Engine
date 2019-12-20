#ifndef _ResourceGraph_H_
#define _ResourceGraph_H_

#include "Resource.h"

struct Node
{
	Node() {};
	~Node() {};

	uint node_id = 0;

};

class ResourceGraph : public Resource
{
public:
	ResourceGraph(uint UID = 0);
	~ResourceGraph();

};

#endif // !_ResourceGraph_H_
