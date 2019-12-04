#include <Section/Sections.h>

Sections::Sections()
{
	hello = make_unique<Hello>();
	pbrLevel = make_unique<PBR_Section>();
	modelLevel = make_unique<Model_Section>();
}

Sections::~Sections()
{

}
void Sections::SetSection(SectionEnum selectedSection)
{
	cureentSection = selectedSection;
	pbrLevel->Reset();
	modelLevel->Reset();

	switch (cureentSection) {
	case SectionEnum::None:
		break;
	case SectionEnum::BlinnPhong:
		break;
	case SectionEnum::LoadModel:
		Level::currentLevel = modelLevel.get();
		break;
	case SectionEnum::PBR:
		Level::currentLevel = pbrLevel.get();
		break;
	default:
		break;
	}
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
			modelLevel->Loop();
            break;
        case SectionEnum::PBR:
			pbrLevel->Loop();
            break;
        default:
            break;
    }
}

