//
//  Transform.cpp
//  chaoGL
//
//  Created by Chao on 2019/3/23.
//

#include "Transform.h"

Transform::Transform() : pos(glm::vec3(0.0f)), scale(glm::vec3(0.0f)), rotation(glm::vec3(0.0f))
{

}

Transform::Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale) : pos(pos), scale(scale), rotation(rotation)
{

};

Transform::~Transform()
{

}
