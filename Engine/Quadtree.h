#ifndef _Quadtree_H_
#define _Quadtree_H_

#define MAX_ITEMS 2
#define MAX_DIVISIONS 5
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
	template<typename Type>
	void Intersect(std::map<float, GameObject*>& _game_objects, Type& _primitive);

	void Draw();

public:

	math::AABB bounding_box;
	std::vector<GameObject*> game_objects;

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
	template<typename Type>
	void Intersect(std::map<float, GameObject*>& _game_objects, const Type& _primitive);

	void Draw();

private:
	QuadtreeNode * root = nullptr;

};

template<typename Type>
inline void QuadtreeNode::Intersect(std::vector<GameObject*> &_game_objects, Type& _primitive)
{
	if (_primitive.Intersects(bounding_box))
	{
		for (std::vector<GameObject*>::const_iterator iterator = game_objects.begin(); iterator != game_objects.end(); ++iterator)
		{
			if (_primitive.Intersects((*iterator)->obb))
			{
				if (std::find(_game_objects.begin(), _game_objects.end(), *iterator) == _game_objects.end())
					_game_objects.push_back(*iterator);
			}
		}

		if (!IsLeaf())
		{
			for (uint i = 0; i < CHILDRENS; ++i)
				children[i]->Intersect(_game_objects, _primitive);
		}
	}
}

template<typename Type>
inline void QuadtreeNode::Intersect(std::map<float,  GameObject*>& _game_objects, Type& _primitive)
{
	if (_primitive.Intersects(bounding_box))
	{
		for (std::vector<GameObject*>::const_iterator iterator = game_objects.begin(); iterator != game_objects.end(); ++iterator)
		{
			float distance_near, distance_far;
			if (_primitive.Intersects((*iterator)->obb, distance_near, distance_far))
			{
			_game_objects.emplace(distance_near, *iterator);
			}
		}

		if (!IsLeaf())
		{
			for (uint i = 0; i < CHILDRENS; ++i)
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

template<typename Type>
inline void Quadtree::Intersect(std::map<float, GameObject*>& _game_objects, const Type& _primitive)
{
	root->Intersect(_game_objects, _primitive);
}

#endif // !__Quadtree_H__
