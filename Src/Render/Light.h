#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include<Render/Geo.h>

class Light
{
public:
	Light(glm::vec3 pos);
	~Light();


private:

	glm::vec3 pos = glm::vec3(0,0,0);

	unsigned int lightVAO;


	// mesh
	


};


#endif
