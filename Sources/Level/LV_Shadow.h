#ifndef  LV_SHADOW_H
#define LV_SHADOW_H

#include<Level/LV_Basic_PBR.h>

class LV_Shadow : public LV_Basic_PBR
{
public:

	LV_Shadow() : LV_Basic_PBR("/Assets/Texture/HDR/Mans_Outside_2k.hdr")
	{

	}

	virtual void Loop() override;
	virtual void OnGui() override;
protected:

	virtual void Initialize() override;

private:

	void DirectionalShadow();
	void PointShadow();

private:

	unique_ptr<Model> m_colaModel = nullptr;
	unique_ptr<Model> m_groudModel = nullptr;
	unique_ptr<Model> m_shaderballModel_spec = nullptr;

	unique_ptr<Model> basicGeo_Cone = nullptr;
	unique_ptr<Model> basicGeo_Cube = nullptr;
	unique_ptr<Model> basicGeo_Cylinder = nullptr;
	unique_ptr<Model> basicGeo_Helix = nullptr;
	unique_ptr<Model> basicGeo_Pipe = nullptr;
	unique_ptr<Model> basicGeo_Prism = nullptr;
	unique_ptr<Model> basicGeo_Pyramid = nullptr;
	unique_ptr<Model> basicGeo_Sphere = nullptr;
	unique_ptr<Model> basicGeo_Torus = nullptr;

	unique_ptr<Model> cuberoomModel = nullptr;

	unique_ptr<Material> m_mat_ground = nullptr;
	unique_ptr<Material> m_mat_shaderball_spec = nullptr;
	unique_ptr<Material> m_mat_cola = nullptr;
};


#endif // ! LV_SHADOW_H
