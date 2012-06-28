#pragma once

#include <vector>
#include <iostream>

template<class T>
class IEntityListener
{
public:
	virtual void OnEntityAdded(T ent) {};
	virtual void OnEntitySpawned(T ent) {};
	virtual void OnEntityRemoved(T ent) {};
};


template<class T>
class EntityList
{
public:
	typedef T VType;
	typedef std::vector<VType> Container;
	typedef typename Container::iterator iter;
private:
	Container mList;
	std::vector<IEntityListener<T>*> mListeners;
public:
	EntityList();
	~EntityList();

	void Append(VType ent);
	void InsertAt(VType ent, iter pos);
	void Clear();
	void ClearDontDelete();

	int GetSize() {return mList.size();};

	void RegisterListener(IEntityListener<T> *listen);

	iter DeleteHard(iter pos);
	iter DeleteSoft(iter pos);

	iter FirstEnt();
	iter NextEnt(iter CurrentIter);
	iter PreviousEnt(iter CurrentIter);
	iter End();
};

/*
NAME	: Constructor
NOTES	: 
*/
template<class T>
EntityList<T>::EntityList(void)
{
}

/*
NAME	: Destructor
NOTES	: 
*/
template<class T>
EntityList<T>::~EntityList(void)
{
	mList.clear();
}

template<class T>
void EntityList<T>::InsertAt(VType ent, iter pos)
{
	mList.insert(pos, ent);
}

/*
NAME	: Clear
NOTES	: Delete all the entities from memory in our list
*/
template<class T>
void EntityList<T>::Clear()
{
	iter CurEnt = FirstEnt();
	while (mList.size() > 0)
	{
		delete *CurEnt;
		CurEnt = mList.erase(CurEnt);
	}
}

/*
NAME	: ClearDontDelete
NOTES	: Delete all the entities from our list keep the objects
*/
template<class T>
void EntityList<T>::ClearDontDelete()
{
	mList.clear();
}

/*
NAME	: RegisterListener
NOTES	: Adds an entity listener to the list. Whenever an entity is
		  created or destroyed in the list the listener will get called
*/
template<class T>
void EntityList<T>::RegisterListener(IEntityListener<T> *listen)
{
	mListeners.push_back(listen);
}

/*
NAME	: Append
NOTES	: Adds an entity to the list
*/
template<class T>
void EntityList<T>::Append(VType ent)
{
	mList.push_back(ent);
	typename std::vector<IEntityListener<T>*>::iterator i;
	i = mListeners.begin();
	while (i != mListeners.end())
	{
		(*i)->OnEntityAdded(ent);
		i++;
	}
}

/*
NAME	: DeleteHard
NOTES	: Deletes a specific entity from memory and the list
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::DeleteHard(iter pos)
{
	typename std::vector<IEntityListener<T>*>::iterator i = mListeners.begin();
	while (i != mListeners.end())
	{
		(*i)->OnEntityRemoved(*pos);
		i++;
	}
	delete *pos;
	return mList.erase(pos);
}

/*
NAME	: DeleteSoft
NOTES	: Deletes a specific entity from the list, NOT FROM MEMORY
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::DeleteSoft(iter pos)
{
	typename std::vector<IEntityListener<T>*>::iterator i = mListeners.begin();
	while (i != mListeners.end())
	{
		(*i)->OnEntityRemoved(*pos);
		i++;
	}
	return mList.erase(pos);
}

/*
NAME	: FirstEnt
NOTES	: Get the first entity in our list, revalidate the iterator
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::FirstEnt()
{
	return mList.begin();
}

/*
NAME	: NextEnt
NOTES	: Get the next entity unless there isnt one -> return null
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::NextEnt(iter CurrentIter)
{
	CurrentIter++;
	return CurrentIter;
}

/*
NAME	: PreviousEnt
NOTES	: Get the previous entity unless there isnt one -> return null
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::PreviousEnt(iter CurrentIter)
{
	CurrentIter--;
	return CurrentIter;
}

/*
NAME	: End
NOTES	: Get the previous entity unless there isnt one -> return null
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::End()
{
	return mList.end();
}