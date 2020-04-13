//
//  BaseObject.h
//  chaoGL
//
//  Created by Chao on 2019/3/23.
//

#ifndef Transform_h
#define Transform_h

#include <stdio.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Transform
{
public:
	Transform();
    Transform(glm::vec3 pos, glm::vec3 rotation = glm::vec3(0,0,0), glm::vec3 scale = glm::vec3(0,0,0));
    ~Transform();

public:

	virtual void Move(glm::vec3 pos)
	{
		this->pos = pos;
	}
	virtual void Move(float x, float y, float z)
	{
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
	}
	virtual void Translate(glm::vec3 translate)
	{
		this->pos += translate;
	}
	virtual void Translate(float x, float y, float z)
	{
		this->pos.x += x;
		this->pos.y += y;
		this->pos.z += z;
	}
	virtual void SetRotation(glm::vec3 rotation)
	{
		this->rotation = rotation;
	}
	virtual void SetRotation(float x, float y, float z)
	{
		this->rotation.x = x;
		this->rotation.y = y;
		this->rotation.z = z;
	}
	virtual void Rotate(glm::vec3 rotate)
	{
		this->rotation.x += rotate.x;
		this->rotation.y += rotate.y;
		this->rotation.z += rotate.z;
	}
	virtual void Rotate(float x ,float y, float z)
	{
		this->rotation.x += x;
		this->rotation.y += y;
		this->rotation.z += z;
	}
	virtual void Scale(glm::vec3 scale)
	{
		this->scale.x = scale.x;
		this->scale.y = scale.y;
		this->scale.z = scale.z;
	}
	virtual void Scale(float x, float y, float z)
	{
		this->scale.x = x;
		this->scale.y = y;
		this->scale.z = z;
	}
	virtual void AddScale(glm::vec3 scale)
	{
		this->scale.x *= scale.x;
		this->scale.y *= scale.y;
		this->scale.z *= scale.z;
	}
	virtual void AddScale(float x, float y, float z)
	{
		this->scale.x *= x;
		this->scale.y *= y;
		this->scale.z *= z;
	}


	virtual glm::vec3 GetPos() { return pos; }
	virtual glm::vec3 GetRotate() { return rotation; }
	virtual glm::vec3 GetScale() { return scale; }



protected:

	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rotation;

private:

    
    
};

#endif /* Transform_h */
