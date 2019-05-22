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
class Transform
{
public:
    Transform();
    Transform(glm::vec3 pos, glm::vec3 rotation = glm::vec3(0,0,0), glm::vec3 scale = glm::vec3(0,0,0));
    ~Transform();
    

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


protected:

    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rotation;
//    glm::vec3 GetPos() const {return m_pos;}
//    glm::vec3 GetRotation() const {return m_rotation;}
//    glm::vec3 GetScale() const {return m_scale;}
 
    
public:
    
    
};

#endif /* Transform_h */
