#pragma once
#include "NonCopyable.h"

template <class T>
class Singleton : public NonCopyable
{
public:

	// Renvoie l'instance unique de la classe
    static T& GetInstance()
    {
        if (!mInst)
            mInst = new T;

        return *mInst;
    }

	// Détruit l'instance unique de la classe (attention aux autres références !)
    static void Destroy()
    {
        delete mInst;
        mInst = nullptr;
    }

protected:
	// Ctor & Dtor
    Singleton() {}
    ~Singleton() {}

private :
	// Instance de la classe
    static T* mInst;
};

// Déclaration de l'instance que est statique
template <class T> T* Singleton<T>::mInst = nullptr;