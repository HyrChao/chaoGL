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
	void DeleteID();

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
	DeleteID();
}

template<class T> unsigned int& UniqueID<T>::GetID()
{
	return ID;
}

template<class T> void UniqueID<T>::GenerateID()
{
	// consider to replace id to UUID
	++lastID;
	ID = lastID;
	chachedID.push_back(ID);
}

template<class T> void UniqueID<T>::DeleteID()
{
	if (!chachedID.empty())
	{
		for (auto i = chachedID.begin(); i != chachedID.end(); i++)
		{
			unsigned int currentID = *i;
			if (currentID == ID)
			{
				chachedID.erase(i);
				break;
			}
		}
	}
	else
	{
	}
}

#endif // !UNIQUEID_H

