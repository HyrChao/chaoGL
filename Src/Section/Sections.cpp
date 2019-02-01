#include <Section\Sections.h>

Sections::Sections()
{
	hello = new Hello();
}

Sections::~Sections()
{
	delete hello;
}

void Sections::HelloTriangle()
{
	hello->HelloTriangle();
}