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

	// D�truit l'instance unique de la classe (attention aux autres r�f�rences !)
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

// D�claration de l'instance que est statique
template <class T> T* Singleton<T>::mInst = nullptr;