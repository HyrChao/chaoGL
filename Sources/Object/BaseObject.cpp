//
//  BaseObject.cpp
//  chaoGL
//
//  Created by Chao on 2019/3/23.
//

#include "BaseObject.h"

BaseObject::BaseObject()
{
    this->pos =  glm::vec3(0,0,0);
    this->rotation =  glm::vec3(0,0,0);
    this->scale =  glm::vec3(0,0,0);
}

BaseObject::BaseObject(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
{
    this->pos = pos;
    this->rotation = rotation;
    this->scale = scale;
};

BaseObject::~BaseObject()
{
    delete this;
}
