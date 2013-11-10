#pragma once
#include "NonCopyable.h"

#include <mutex>

template <class T>
class Singleton : public NonCopyable
{
public:
	// Renvoie l'instance unique de la classe
    static T& GetInstance()
    {
		// Pas de multithread ici
		static std::mutex mutex;
		std::lock_guard<std::mutex> lock(mutex);

		static T* inst = nullptr;

        if (!inst)
            inst = new T;

        return *inst;
	}
};
