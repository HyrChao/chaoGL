
#ifndef MODEL_SECTION_H
#define MODEL_SECTION_H

#include<Section/PBR_Basic.h>

class Model_Section : PBR_Basic
{
public:
	Model_Section() : PBR_Basic("/Assets/Texture/HDR/Mans_Outside_2k.hdr")
	{

	}

	~Model_Section()
	{
		delete model;
	}


	void Loop() override;

private:



	Model* model = nullptr;

};

#endif // !MODEL_SECTION_H


