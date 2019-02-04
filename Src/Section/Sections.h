#ifndef SECTIONS_H
#define SECTIONS_H

#include <Section\Hello.h>


class Sections
{
public:
	Sections();
	~Sections();
	void SwitchSections();

private:

	Hello* hello;

	bool enterKeyPressing = false;
	bool defaultSection = true;

};

#endif // !SECTIONS_H