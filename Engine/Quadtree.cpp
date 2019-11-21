#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Quadtree.h"

#define TOP_RIGHT 0
#define TOP_LEFT 1
#define BOTTOM_RIGHT 2
#define BOTTOM_LEFT 3

// QuadtreeNode
QuadtreeNode::QuadtreeNode(const math::AABB& _bounding_box) :bounding_box(_bounding_box)
{
	for (uint i = 0; i < CHILDRENS; ++i)
		children[i] = nullptr;
}

QuadtreeNode::~QuadtreeNode()
{
	if (!IsLeaf())
	{
		for (uint i = 0; i < CHILDRENS; ++i)
			RELEASE(children[i]);
	}
}

bool QuadtreeNode::IsLeaf() const
{
	return children[0] == nullptr;
}

void QuadtreeNode::Insert(GameObject* gameObject)
{
	if (IsLeaf() && game_objects.size() < MAX_ITEMS)
		game_objects.push_back(gameObject);
	else
	{
		game_objects.push_back(gameObject);

		if (subdivision < MAX_DIVISIONS)
		{
			if (IsLeaf())
				Subdivide();

			RedistributeChildren();
		}
	}
}

void QuadtreeNode::Subdivide()
{
	const math::float3 size = bounding_box.Size();
	const math::float3 halfSize(size.x / 2.0f, size.y, size.z / 2.0f);
	const math::float3 quarterSize(halfSize.x / 2.0f, size.y, halfSize.z / 2.0f);

	const math::float3 center = bounding_box.CenterPoint();
	math::float3 quarterCenter;

	math::AABB quarterBoundingBox;

	// North East
	quarterCenter = { center.x + quarterSize.x, center.y, center.z - quarterSize.z };
	quarterBoundingBox.SetFromCenterAndSize(quarterCenter, halfSize);
	children[TOP_RIGHT] = new QuadtreeNode(quarterBoundingBox);

	// North West
	quarterCenter = { center.x - quarterSize.x, center.y, center.z - quarterSize.z };
	quarterBoundingBox.SetFromCenterAndSize(quarterCenter, halfSize);
	children[TOP_LEFT] = new QuadtreeNode(quarterBoundingBox);

	// South East
	quarterCenter = { center.x + quarterSize.x, center.y, center.z + quarterSize.z };
	quarterBoundingBox.SetFromCenterAndSize(quarterCenter, halfSize);
	children[BOTTOM_RIGHT] = new QuadtreeNode(quarterBoundingBox);

	// South West
	quarterCenter = { center.x - quarterSize.x, center.y, center.z + quarterSize.z };
	quarterBoundingBox.SetFromCenterAndSize(quarterCenter, halfSize);
	children[BOTTOM_LEFT] = new QuadtreeNode(quarterBoundingBox);

	children[TOP_RIGHT]->subdivision = children[TOP_LEFT]->subdivision = children[BOTTOM_RIGHT]->subdivision = children[BOTTOM_LEFT]->subdivision = subdivision + 1;
}

void QuadtreeNode::RedistributeChildren()
{
	std::list<GameObject*>::const_iterator it = game_objects.begin();

	while (it != game_objects.end())
	{
		uint intersections = 0;
		uint lastIntersection = 0;
		for (uint i = 0; i < CHILDRENS; ++i)
		{
			if ((*it)->abb.Intersects(children[i]->bounding_box))
			{
				++intersections;
				lastIntersection = i;
			}
		}

		if (intersections == 1)
		{
			children[lastIntersection]->Insert(*it);
			it = game_objects.erase(it);
		}
		else if (intersections == CHILDRENS)
		{
			++it;
		}
		else
		{
			for (uint i = 0; i < CHILDRENS; ++i)
			{
				if ((*it)->abb.Intersects(children[i]->bounding_box))
					children[i]->Insert(*it);
			}
			it = game_objects.erase(it);
		}
	}
}

void QuadtreeNode::Draw()
{
	float3 corners[8];
	bounding_box.GetCornerPoints(corners);
	App->renderer3D->DebugDrawCube(corners, { 1.0f, 0.0f, 0.0f });

	if (children[0]) {
		for (int i = 0; i < CHILDRENS; i++) {
			children[i]->Draw();
		}
	}
}

// Quadtree
Quadtree::Quadtree() 
{

}

Quadtree::~Quadtree()
{
	Clear();
}

void Quadtree::Create(const math::AABB& limits)
{
	Clear();
	root = new QuadtreeNode(limits);
}

void Quadtree::Clear()
{
	RELEASE(root);
}

void Quadtree::Insert(GameObject* _game_object)
{
	if (_game_object->abb.Intersects(root->bounding_box))
		root->Insert(_game_object);
}

void Quadtree::Draw()
{
	root->Draw();
}