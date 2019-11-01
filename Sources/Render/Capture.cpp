
#include <Render/Capture.h>


unsigned int Capture::captureFBO;
unsigned int Capture::captureRBO;

glm::mat4 Capture::captureProjection;
glm::mat4 Capture::captureViewMats[6];

Material* Capture::equirectangularToCubemapMaterial;
Material* Capture::irradianceConvolveMaterial;
Material* Capture::specularPrefilterMaterial;
Material* Capture::prefilterBRDFMaterial;

Capture::Capture()
{

}

Capture::~Capture()
{

}

