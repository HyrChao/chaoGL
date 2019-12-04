#ifndef SECTIONS_H
#define SECTIONS_H

#include <Section/Hello.h>
#include <Section/PBR_Section.h>
#include <Section/Model_Section.h>
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

	unique_ptr<Hello> hello;
	unique_ptr<Level> pbrLevel;
	unique_ptr<Level> modelLevel;
    SectionEnum cureentSection = SectionEnum::LoadModel;


};


#endif // !SECTIONS_H
