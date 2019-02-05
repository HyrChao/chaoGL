#include <Section\Sections.h>

Sections::Sections()
{
	hello = new Hello();
}

Sections::~Sections()
{
	delete hello;
}
void Sections::SetDefaultSection(bool firstDefault)
{
	defaultSection = firstDefault;
}
bool Sections::GetDefaultSection()
{
	return defaultSection;
}

void Sections::SwitchSections()
{

	if (defaultSection)
	{
		hello->HelloBox();
	}
	else
	{
		hello->HelloCamera();
	}
}

