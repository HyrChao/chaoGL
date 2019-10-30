#ifndef PBR_BASIC_H
#define PBR_BASIC_H


#include <Level/Level.h>
#include<Input/Input.h>


class PBR_Basic : public Level
{
public:
	PBR_Basic() : Level()
	{

	}
	PBR_Basic(string skyHDRPath) : Level(skyHDRPath)
	{
		
	}

	~PBR_Basic();

protected:

	virtual void Loop();

	glm::vec4 pbrDebugParam = glm::vec4(0);
	glm::vec4 lightDebugParam = glm::vec4(0);

private:

	void PBRMaterialDebug();
};

#endif // !PBR_BASIC_H

