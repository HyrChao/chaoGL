#pragma once

#include <glm.hpp>
#include <map>

class Material;

struct DrawableContext
{
	DrawableContext() :
		material(nullptr), modelMat(glm::mat4(1.0f))
	{

	}
	DrawableContext(Material* material, glm::mat4 modelMat) :
		material(material), modelMat(modelMat)
	{

	}
	Material* material;
	glm::mat4 modelMat;
};

class IDrawable;

class DrawableList : public std::map<IDrawable*, DrawableContext>
{
public:

	DrawableList() = default;
	virtual ~DrawableList() = default;

	Material* GetMaterial(IDrawable* drawable) const;

	void SetMaterial(IDrawable* drawable, Material* mtl);

	void SetModelMat(IDrawable* drawable, glm::mat4& mat);

};

class IDrawable
{

public:

	virtual void Draw(Material* material) const = 0;
	virtual void Draw(Material* material, glm::mat4& modelMat) const = 0;
	virtual void Bind(DrawableList& drawablelist, DrawableContext drawablecontext) const = 0;
	virtual void Bind(DrawableList& drawablelist, Material* material, glm::mat4 modelMat) const = 0;

	virtual ~IDrawable() = default;

private:

};



