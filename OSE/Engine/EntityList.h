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
	iter mIter;
	bool mCurIsValid;
public:
	EntityList();
	~EntityList();

	void Append(VType ent);
	void InsertAtCurrent(VType);
	void Clear();
	void ClearDontDelete();

	bool CurrentIsValid();

	int GetSize() {return mList.size();};

	void RegisterListener(IEntityListener<T> *listen);

	void DeleteCurrent();
	void Delete(T ent);

	typename std::vector<T>::iterator FirstEnt();
	iter CurrentEnt();
	iter NextEnt(iter CurrentIter);
	iter PreviousEnt(iter CurrentIter);
};

/*
NAME	: Constructor
NOTES	: 
*/
template<class T>
EntityList<T>::EntityList(void)
{
	mIter = mList.begin();
	mCurIsValid = false;
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

/*
NAME	: CurrentIsValid
NOTES	: If the current iterator points to a valid entity. This is a
		  bit of a hack but its stable right now if you iterate in a
		  specific way.
*/
template<class T>
bool EntityList<T>::CurrentIsValid()
{
	return mCurIsValid;
}

/*
NAME	: Clear
NOTES	: Delete all the entities from memory in our list
*/
template<class T>
void EntityList<T>::Clear()
{
	iter CurEnt = FirstEnt();
	while (CurrentIsValid())
	{
		DeleteCurrent();
		CurEnt = CurrentEnt();
	}
	mList.clear();
}

/*
NAME	: ClearDontDelete
NOTES	: Delete all the entities from our list keep the objects
*/
template<class T>
void EntityList<T>::ClearDontDelete()
{
	mList.clear();
	mCurIsValid = false;
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
NAME	: InsertAtCurrent
NOTES	: Insert an entity at the current iteration
*/
template<class T>
void EntityList<T>::InsertAtCurrent(VType ent)
{
	//assert(CurrentIsValid());
	mList.insert(mIter, ent);

	typename std::vector<IEntityListener<T>*>::iterator i;
	i = mListeners.begin();
	while (i != mListeners.end())
	{
		(*i)->OnEntityAdded(ent);
		i++;
	}
}

/*
NAME	: Delete
NOTES	: Deletes a specific entity from the list
*/
template<class T>
void EntityList<T>::Delete(T ent)
{
	iter i = mList.begin();
	bool found = false;
	while (!found && i != mList.end())
	{
		if ((*i) == ent)
		{
			//Inform Listeners
			typename std::vector<IEntityListener<T>*>::iterator i2;
			i2 = mListeners.begin();
			while (i2 != mListeners.end())
			{
				(*i2)->OnEntityRemoved(ent);
				i2++;
			}
			//Delete the entity
			mList.erase(i);
			found = true;
		}
		i++;
	}
}

/*
NAME	: DeleteCurrent
NOTES	: Deletes whatever entity we are currently iterating at
		  IMPORTANT: also changes mIter to be the NEW next
		  entity. If you use DeleteCurrent in an iteration through
		  a list then refresh your CurEnt with GetCurrent()
*/
template<class T>
void EntityList<T>::DeleteCurrent()
{
	//Inform Listeners
	typename std::vector<IEntityListener<T>*>::iterator i;
	i = mListeners.begin();
	while (i != mListeners.end())
	{
		(*i)->OnEntityRemoved(*mIter);
		i++;
	}
	//Delete Entity
	delete *mIter;
	mIter = mList.erase(mIter);
	if (mIter >= mList.end())
		mCurIsValid = false;
}

/*
NAME	: FirstEnt
NOTES	: Get the first entity in our list, revalidate the iterator
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::FirstEnt()
{
	iter FirstIter;
	if (mList.size() == 0)
	{
		mCurIsValid = false;
	}
	mCurIsValid = true;
	FirstIter = mList.begin();
	return FirstIter;
}

/*
NAME	: CurrentEnt
NOTES	: 
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::CurrentEnt()
{
	return mIter;
}

/*
NAME	: NextEnt
NOTES	: Get the next entity unless there isnt one -> return null
*/
template<class T>
typename std::vector<T>::iterator EntityList<T>::NextEnt(iter CurrentIter)
{
	CurrentIter++;
	if (mIter >= mList.end())
	{
		mCurIsValid = false;
	}
	mCurIsValid = true;
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
	if (mIter <= mList.begin())
	{
		mCurIsValid = false;
	}
	mCurIsValid = true;
	return CurrentIter;
}