#include <Section/Sections.h>

Sections::Sections()
{
	hello = new Hello();
}

Sections::~Sections()
{
	delete hello;
}
void Sections::SetSection(SectionEnum selectedSection)
{
	cureentSection = selectedSection;
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
            hello->HelloPBR();
            break;
        default:
            break;
    }
}

