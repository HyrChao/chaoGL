#ifndef SECTIONS_H
#define SECTIONS_H

#include <Section/Hello.h>


class Sections
{
public:
	Sections();
	~Sections();
	void SwitchSections();
	void SetDefaultSection(bool firstDefault);
	bool GetDefaultSection();

private:

	Hello* hello;
	bool defaultSection = true;


};

#endif // !SECTIONS_H
