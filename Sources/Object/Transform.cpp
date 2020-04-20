//
//  Transform.cpp
//  chaoGL
//
//  Created by Chao on 2019/3/23.
//

#include "Transform.h"

Transform::Transform() : 
	pos(glm::vec3(0.0f)), 
	scale(glm::vec3(1.0f)), 
	rotation(glm::vec3(0.0f)), 
	modelMat(glm::mat4(1.0f)),
	prev_pos(glm::vec3(0.0f)),
	prev_scale(glm::vec3(1.0F)),
	prev_rotation(glm::vec3(0.0F))
{

}

Transform::Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale) : 
	pos(pos), 
	scale(scale), 
	rotation(rotation), 
	modelMat(glm::mat4(1.0f)),
	prev_pos(glm::vec3(0.0f)),
	prev_scale(glm::vec3(1.0f)),
	prev_rotation(glm::vec3(0.0f))
{
	UpdatePositionToMat();
	UpdateRotationToMat();
	UpdateScaleToMat();
};

Transform::~Transform()
{

}

inline void Transform::MoveTo(glm::vec3 pos)
{
	this->pos = pos;
	UpdatePositionToMat();
}

inline void Transform::MoveTo(float x, float y, float z)
{
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
	UpdatePositionToMat();
}

inline void Transform::Translate(glm::vec3 translate)
{
	this->pos += translate;
	UpdatePositionToMat();
}

inline void Transform::Translate(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	UpdatePositionToMat();
}

inline void Transform::SetRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
	UpdateRotationToMat();
}

inline void Transform::SetRotation(float x, float y, float z)
{
	this->rotation.x = x;
	this->rotation.y = y;
	this->rotation.z = z;
	UpdateRotationToMat();
}

inline void Transform::Rotate(glm::vec3 rotate)
{
	this->rotation.x += rotate.x;
	this->rotation.y += rotate.y;
	this->rotation.z += rotate.z;
	UpdateRotationToMat();
}

inline void Transform::Rotate(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	UpdateRotationToMat();
}

inline void Transform::SetScale(glm::vec3 scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;
	this->scale.z = scale.z;
	UpdateScaleToMat();
}

inline void Transform::SetScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
	UpdateScaleToMat();
}

inline void Transform::Scale(glm::vec3 scale)
{
	this->scale.x *= scale.x;
	this->scale.y *= scale.y;
	this->scale.z *= scale.z;
	UpdateScaleToMat();
}

inline void Transform::Scale(float x, float y, float z)
{
	this->scale.x *= x;
	this->scale.y *= y;
	this->scale.z *= z;
	UpdateScaleToMat();
}

inline void Transform::UpdateRotationToMat()
{
	glm::vec3 delta_rotation = rotation - prev_rotation;
	this->modelMat = glm::translate(this->modelMat, -pos);
	this->modelMat = glm::rotate(this->modelMat, delta_rotation.x, glm::vec3(1.0, 0.0, 0.0));
	this->modelMat = glm::rotate(this->modelMat, delta_rotation.y, glm::vec3(0.0, 1.0, 0.0));
	this->modelMat = glm::rotate(this->modelMat, delta_rotation.z, glm::vec3(0.0, 0.0, 1.0));
	this->modelMat = glm::translate(this->modelMat, pos);
	prev_rotation = rotation;
}

inline void Transform::UpdatePositionToMat()
{
	glm::vec3 translateAmout = (pos - prev_pos) / scale;
	this->modelMat = glm::rotate(this->modelMat, -rotation.x, glm::vec3(1.0, 0.0, 0.0));
	this->modelMat = glm::rotate(this->modelMat, -rotation.y, glm::vec3(0.0, 1.0, 0.0));
	this->modelMat = glm::rotate(this->modelMat, -rotation.z, glm::vec3(0.0, 0.0, 1.0));
	this->modelMat = glm::translate(modelMat, translateAmout);
	this->modelMat = glm::rotate(this->modelMat, rotation.x, glm::vec3(1.0, 0.0, 0.0));
	this->modelMat = glm::rotate(this->modelMat, rotation.y, glm::vec3(0.0, 1.0, 0.0));
	this->modelMat = glm::rotate(this->modelMat, rotation.z, glm::vec3(0.0, 0.0, 1.0));
	prev_pos = pos;
}

inline void Transform::UpdateScaleToMat()
{
	glm::vec3 delta_scale = scale / prev_scale;
	this->modelMat = glm::translate(this->modelMat,-pos);
	this->modelMat = glm::scale(modelMat, delta_scale);
	this->modelMat = glm::translate(this->modelMat, pos);
	prev_scale = scale;
}
