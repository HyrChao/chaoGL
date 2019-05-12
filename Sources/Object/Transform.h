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
    
//    glm::vec3 GetPos() const {return m_pos;}
//    glm::vec3 GetRotation() const {return m_rotation;}
//    glm::vec3 GetScale() const {return m_scale;}
 
    
public:
    
    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rotation;
    
};

#endif /* Transform_h */
