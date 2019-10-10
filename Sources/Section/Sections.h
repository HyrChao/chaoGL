#ifndef SECTIONS_H
#define SECTIONS_H

#include <Section/Hello.h>
#include <Section/PBR_Section.h>
enum SectionEnum
{
    None,
    BlinnPhong,
    LoadModel,
    PBR
};

class Sections
{
public:
	Sections();
	~Sections();
	void SwitchSections();
	void SetSection(SectionEnum selectedSection);
	SectionEnum GetCurrentSection();

private:

	Hello* hello;
	PBR_Section* pbr_section;
    SectionEnum cureentSection = SectionEnum::PBR;


};


#endif // !SECTIONS_H
