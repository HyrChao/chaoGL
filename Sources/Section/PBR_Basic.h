#ifndef PBR_BASIC_H
#define PBR_BASIC_H


#include <Level/Level.h>
#include<Input/Input.h>


class PBR_Basic : public Level
{
public:
	PBR_Basic();
	PBR_Basic(string skyHDRPath);

	virtual ~PBR_Basic();

	virtual void Loop() override;

protected:

	virtual void LoadLevelResource() override = 0;
	virtual void Initialize() override;

	glm::vec4 pbrDebugParam = glm::vec4(0);
	glm::vec4 lightDebugParam = glm::vec4(0);

	void RegisterPBRShader(Shader* shader);

	void DeregisterPBRShader(Shader* shader);

	// Call after shader registered 
	void InitPBR();

	void BindPBRTextures();

	void DeregisterAllPBRShader();

	vector<Shader*> pbrShaders;


private:

	void PBRMaterialDebug();


};

#endif // !PBR_BASIC_H

