#pragma once
#include "Level.h"
#include <memory>

class LV_PostFX : public Level
{
public:

	LV_PostFX();

	void Loop() override;

	void OnGui() override;

protected:

	void Initialize() override;

	virtual void UpdatePostFXParameters();

private:

	unique_ptr<Shader> s_starTravelling;

};

