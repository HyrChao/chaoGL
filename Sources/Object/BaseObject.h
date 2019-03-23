//
//  BaseObject.h
//  chaoGL
//
//  Created by Chao on 2019/3/23.
//

#ifndef BaseObject_h
#define BaseObject_h

#include <stdio.h>
#include <chaoGL.h>
class BaseObject
{
public:
    BaseObject();
    BaseObject(glm::vec3 pos, glm::vec3 rotation = glm::vec3(0,0,0), glm::vec3 scale = glm::vec3(0,0,0));
    ~BaseObject();
    
//    glm::vec3 GetPos() const {return m_pos;}
//    glm::vec3 GetRotation() const {return m_rotation;}
//    glm::vec3 GetScale() const {return m_scale;}
 
    
public:
    
    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rotation;
    
};

#endif /* BaseObject_h */
