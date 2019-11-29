#ifndef CAPTURE_H
#define CAPTURE_H

#include<Render/Material.h>
#include<Assets/CommonAssets.h>
#include <Render/RenderDevice.h>

class Capture
{

public:

	Capture();
	~Capture();

	static void InitCapture();

	static void CaptureIrradianceCubemap(Texture& envCubemap, Texture& irradianceMap);

	static void CaptureSpecularPrefilterMap(Texture&envCubemap, Texture& prefilterEnvironmentMap);

	static void CaptureEnvironmentCubemap(Texture& equirectangularMap, Texture& envCubemap);

	static void PrefilterBRDF(Texture& brdfLUT);

private:

	// capture view mats from origin
	static glm::mat4 captureViewMats[6];

	static unsigned int captureFBO, captureRBO;

	// captue projection mat
	static glm::mat4 captureProjection;

	// material for convert equirectangular map tp cubmap 
	static Material* equirectangularToCubemapMaterial;
	// material for convolve irradiance
	static Material* irradianceConvolveMaterial;
	// material for convolve specular 
	static Material* specularPrefilterMaterial;
	// material for prefilter BRDF
	static Material* prefilterBRDFMaterial;

};



#endif // !CAPTURE_H
