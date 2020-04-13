#include <Object/UniqueID.h>

// Nhttps://stackoverflow.com/questions/1639797/template-issue-causes-linker-error-c
//  templates cannot be compiled. Think of functions as cookies, and the compiler is an oven.



//template<class T> UniqueID<T>::UniqueID()
//{
//	GenerateID();
//}
//
//template<class T> UniqueID<T>::~UniqueID()
//{
//	chachedID.push_back(ID);
//}
//
//template<class T> unsigned int& UniqueID<T>::GetID()
//{
//	return ID;
//}
//
//template<class T> void UniqueID<T>::GenerateID()
//{
//	if (!chachedID.empty())
//	{
//		auto i = chachedID.begin();
//		ID = *i;
//		chachedID.pop_front();
//	}
//	else
//	{
//		++lastID;
//		ID = lastID;
//	}
//}
