#ifndef LV_Basic_PBR_H
#define LV_Basic_PBR_H


#include <Level/Level.h>
#include<Input/Input.h>


class LV_Basic_PBR : public Level
{
public:
	LV_Basic_PBR();
	LV_Basic_PBR(string skyHDRPath);

	virtual ~LV_Basic_PBR();

	virtual void Loop() override;
	virtual void OnGui() override;

protected:

	virtual void Initialize() override;

	virtual void CaptureEnvironment() override;

	glm::vec4 pbrDebugParam = glm::vec4(0);
	glm::vec4 lightDebugParam = glm::vec4(0);

	void RegisterPBRShader(Shader* shader);
	void DeregisterPBRShader(Shader* shader);

	void BindPBRTextures();

	void DeregisterAllPBRShader();

	vector<Shader*> pbrShaders;

	// Call after shader registered 
	void SetPBRShaderParams();
	void SetPBRShaderDebugParams();

private:


	void PBRMaterialDebug();


};

#endif // !LV_Basic_PBR_H

