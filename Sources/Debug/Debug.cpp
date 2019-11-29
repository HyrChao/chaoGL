#include "Debug.h"

#ifdef WIN32

#define ASSERT(x) if (!(x)) __debugbreak()

#endif // WIN32


Debug::Debug()
{
}


Debug::~Debug()
{
}
