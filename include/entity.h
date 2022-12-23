#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp> //glm::mat4
#include <list> //std::list
#include <array> //std::array
#include <memory> //std::unique_ptr

#include "model.h"

class Entity
{
public:
	//Scene graph
	std::list<std::unique_ptr<Entity>> children;
	Entity* parent = nullptr;

	//Space information
	Transform transform;

	Model* pModel = nullptr;


	// constructor, expects a filepath to a 3D model.
	Entity(Model& model) : pModel( &model )	{}


	//Add child. Argument input is argument of any constructor that you create. By default you can use the default constructor and don't put argument input.
	template<typename... TArgs>
	void addChild(TArgs&... args)
	{
		children.emplace_back(std::make_unique<Entity>(args...));
		children.back()->parent = this;
	}

	//Update transform if it was changed
	void updateSelfAndChild()
	{
		if (!transform.isDirty())
			return;

		forceUpdateSelfAndChild();
	}

	//Force update of transform even if local space don't change
	void forceUpdateSelfAndChild()
	{
		if (parent)
			transform.computeModelMatrix(parent->transform.getModelMatrix());
		else
			transform.computeModelMatrix();

		for (auto&& child : children)
		{
			child->forceUpdateSelfAndChild();
		}
	}


	void drawSelfAndChild(Shader& ourShader,  unsigned int& total)
	{
        ourShader.setMat4("model", transform.getModelMatrix());
        pModel->Draw(ourShader);
		total++;

		for (auto&& child : children)
		{
			child->drawSelfAndChild(ourShader, total);
		}
	}
};
#endif
