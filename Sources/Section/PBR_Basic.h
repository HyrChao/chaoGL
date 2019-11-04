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

	void RegisterPBRShader(Shader* shader)
	{
		pbrShaders.push_back(shader);
	}
	void DeregisterPBRShader(Shader* shader)
	{
		for (vector<Shader*>::iterator it = pbrShaders.begin(); it != pbrShaders.end(); it++)
			if ((*it) == shader)
				pbrShaders.erase(it);
	}

	// Call after shader registered 
	void InitPBR()
	{
		for (auto it = pbrShaders.begin(); it != pbrShaders.end(); it++)
		{
			Shader* shader = *it;
			shader->use();
			shader->setInt("IBL.irradianceMap", TextureSlot::PBR_Irridiance);
			shader->setInt("IBL.prefilterEnv", TextureSlot::PBR_Prefilter);
			shader->setInt("IBL.BRDFPrefilterMap", TextureSlot::PBR_BRDF);

		}
	}

	void BindPBRTextures()
	{
		glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_BRDF);
		glBindTexture(GL_TEXTURE_2D, prefilterBRDFLUT.id);
		glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_Irridiance);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceCubemap.id);
		glActiveTexture(GL_TEXTURE0 + TextureSlot::PBR_Prefilter);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterEnvironmentMap.id);
	}

	void DeregisterAllPBRShader()
	{
		pbrShaders.clear();
	}

	vector<Shader*> pbrShaders;


private:

	void PBRMaterialDebug();


};

#endif // !PBR_BASIC_H

