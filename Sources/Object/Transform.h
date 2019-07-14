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

	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rotation;
	//    glm::vec3 GetPos() const {return m_pos;}
	//    glm::vec3 GetRotation() const {return m_rotation;}
	//    glm::vec3 GetScale() const {return m_scale;}

	virtual glm::mat4 GetModelMat()
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::rotate(model, rotation.x, glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, scale);
		return model;
	}
	virtual void SetPos(glm::vec3 pos)
	{
		this->pos = pos;
	}
	virtual void SetRotation(glm::vec3 rotation)
	{
		this->rotation = rotation;
	}
	virtual void SetScale(glm::vec3 scale)
	{
		this->scale = scale;
	}
	virtual void Translate(glm::vec3 translate)
	{
		this->pos += translate;
	}

	virtual glm::vec3 GetPos() { return pos; }
	virtual glm::vec3 GetRotate() { return rotation; }
	virtual glm::vec3 GetScale() { return scale; }

	virtual void Rotate(glm::vec3 rotate)
	{
		this->rotation.x += rotate.x;
		this->rotation.y += rotate.y;
		this->rotation.z += rotate.z;
	}
	virtual void Scale(glm::vec3 scale)
	{
		this->scale.x *= scale.x;
		this->scale.y *= scale.y;
		this->scale.z *= scale.z;
	}



    
public:
    
    
};

#endif /* Transform_h */
