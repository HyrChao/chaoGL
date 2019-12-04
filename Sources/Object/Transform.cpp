//
//  Transform.cpp
//  chaoGL
//
//  Created by Chao on 2019/3/23.
//

#include "Transform.h"

Transform::Transform()
{
    this->pos =  glm::vec3(0,0,0);
    this->rotation =  glm::vec3(0,0,0);
    this->scale =  glm::vec3(0,0,0);
}

Transform::Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
{
    this->pos = pos;
    this->rotation = rotation;
    this->scale = scale;
};

Transform::~Transform()
{

}
