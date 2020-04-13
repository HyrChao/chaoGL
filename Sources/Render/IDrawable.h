#pragma once

#include <glm.hpp>
#include <map>
#include<Object/UniqueID.h>

class Material;
class IDrawable;

struct DrawableContext : public UniqueID<DrawableContext>
{
	DrawableContext() :
		material(nullptr), 
		drawable(nullptr), 
		model(nullptr)
	{
	}

	DrawableContext(IDrawable* drawable, Material* material, glm::mat4* model) :
		drawable(drawable), 
		material(material), 
		model(model)
	{
	}

	IDrawable* drawable;
	Material* material;
	glm::mat4* model;

private:
};


class DrawableList : public std::map<unsigned int, DrawableContext>
{
public:

	DrawableList() = default;
	virtual ~DrawableList() = default;

	void AddContext(DrawableContext& context);

};

class IDrawable
{

public:

	virtual void Draw(Material* material) const = 0;
	virtual void Bind(DrawableList& drawablelist, Material* material) const = 0;

	virtual ~IDrawable() = default;

private:

};



