#ifndef _Quadtree_H_
#define _Quadtree_H_

#define MAX_ITEMS 2
#define MAX_DIVISIONS 10
#define CHILDRENS 4

class GameObject;
#include "MathGeoLib\include\Geometry\AABB.h"
#include <list>

class QuadtreeNode
{
public:
	QuadtreeNode(const math::AABB &_bounding_box);
	~QuadtreeNode();

	bool IsLeaf() const;

	void Insert(GameObject* _game_object);
	void Subdivide();
	void RedistributeChildren();

	template<typename Type>
	void Intersect(std::vector<GameObject*> &_game_objects, Type& _primitive);

public:

	math::AABB bounding_box;
	std::list<GameObject*> game_objects;

	QuadtreeNode* parent = nullptr;
	QuadtreeNode* children[4];

	uint subdivision = 0;
};

class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	void Create(const AABB &_limits);
	void Clear();
	void Insert(GameObject* gameobject);

	template<typename Type>
	void Intersect(std::vector<GameObject*> &_game_objects, Type & _primitive);

private:
	QuadtreeNode * root = nullptr;

};

template<typename Type>
inline void QuadtreeNode::Intersect(std::vector<GameObject*> &_game_objects, Type& _primitive)
{
	if (primitive.Intersects(bounding_box))
	{
		for (std::list<GameObject*>::const_iterator iterator = objects.begin(); iterator != objects.end(); ++iterator)
		{
			if (primitive.Intersects((*iterator)->bounding_box))
			{
				if (std::find(_game_objects.begin(), _game_objects.end(), *iterator) == _game_objects.end())
					_game_objects.push_back(*iterator);
			}
		}

		if (!IsNode())
		{
			for (uint i = 0; i < 4; ++i)
				children[i]->Intersect(_game_objects, _primitive);
		}
	}
}

template<typename Type>
inline void Quadtree::Intersect(std::vector<GameObject*> &_game_objects, Type& _primitive)
{
	if (root != nullptr)
		root->Intersect(_game_objects, _primitive);
}

#endif // !__Quadtree_H__
