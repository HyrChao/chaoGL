#include <Section/Sections.h>

Sections::Sections()
{
	hello = new Hello();
	pbr_section = new PBR_Section();
	CommonAssets::instance = new CommonAssets();
}

Sections::~Sections()
{
	delete hello;
	delete pbr_section;
}
void Sections::SetSection(SectionEnum selectedSection)
{
	cureentSection = selectedSection;


	hello->Reset();
	pbr_section->Reset();
}
SectionEnum Sections::GetCurrentSection()
{
	return cureentSection;
}

void Sections::SwitchSections()
{
    switch (cureentSection) {
        case SectionEnum::None:
            break;
        case SectionEnum::BlinnPhong:
            hello->HelloLight();
            break;
        case SectionEnum::LoadModel:
            hello->HelloModel();
            break;
        case SectionEnum::PBR:
			pbr_section->Loop();
            break;
        default:
            break;
    }
}

