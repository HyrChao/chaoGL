//
//  Color.h
//  chaoGL
//
//  Created by Chao on 2019/3/23.
//

#ifndef Color_h
#define Color_h

#include <chaoGL.h>
#include <gtx/color_space.hpp>
class Color
{
public:
    Color();
    
    static glm::vec3 RGB2HSV(glm::vec3 rgbCol)
    {
        return glm::hsvColor(rgbCol);
    }
    static glm::vec3 HSV2RGB(glm::vec3 hsvCol)
    {
        return glm::rgbColor(hsvCol);
    }
    static glm::vec3 GetHue(glm::vec3 col, float saturate, float brightness)
    {
        glm::vec3 lightAvatarCol = RGB2HSV(col);
        lightAvatarCol.b = saturate;
        lightAvatarCol.g *= brightness;
        lightAvatarCol = HSV2RGB(lightAvatarCol);
        return lightAvatarCol;
    }
};


#endif /* Color_h */
