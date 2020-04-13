#ifndef UNIQUEID_H
#define UNIQUEID_H

#include <list>


template<class T> class UniqueID
{
public:

	UniqueID();
	virtual ~UniqueID();

	unsigned int& GetID();

protected:

private:

	void GenerateID();

private:

	unsigned int ID;
	static unsigned int lastID;
	static std::list<unsigned int> chachedID;

};

template<class T> unsigned int UniqueID<T>::lastID = 0;
template<class T> std::list<unsigned int> UniqueID<T>::chachedID;

template<class T> UniqueID<T>::UniqueID()
{
	GenerateID();
}

template<class T> UniqueID<T>::~UniqueID()
{
	chachedID.push_back(ID);
}

template<class T> unsigned int& UniqueID<T>::GetID()
{
	return ID;
}

template<class T> void UniqueID<T>::GenerateID()
{
	if (!chachedID.empty())
	{
		auto i = chachedID.begin();
		ID = *i;
		chachedID.pop_front();
	}
	else
	{
		++lastID;
		ID = lastID;
	}
}

#endif // !UNIQUEID_H

