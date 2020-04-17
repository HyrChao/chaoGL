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

	virtual void MoveTo(glm::vec3 pos);
	virtual void MoveTo(float x, float y, float z);
	virtual void Translate(glm::vec3 translate);
	virtual void Translate(float x, float y, float z);
	virtual void SetRotation(glm::vec3 rotation);
	virtual void SetRotation(float x, float y, float z);
	virtual void Rotate(glm::vec3 rotate);
	virtual void Rotate(float x, float y, float z);
	virtual void SetScale(glm::vec3 scale);
	virtual void SetScale(float x, float y, float z);
	virtual void Scale(glm::vec3 scale);
	virtual void Scale(float x, float y, float z);


	virtual glm::vec3 GetPos() { return pos; }
	virtual glm::vec3 GetRotate() { return rotation; }
	virtual glm::vec3 GetScale() { return scale; }



protected:

	glm::vec3 prev_pos;
	glm::vec3 pos;
	glm::vec3 prev_scale;
	glm::vec3 scale;
	glm::vec3 prev_rotation;
	glm::vec3 rotation;
	glm::mat4 modelMat;

private:

	void UpdateRotationToMat();

	void UpdatePositionToMat();

	void UpdateScaleToMat();
    
    
};

#endif /* Transform_h */
