#pragma once

#define REGISTER_VECTOR_WRAPPER(T, vector_T) \
	luabind::class_<vector_T>("vector_b2Vec2") \
		.def(luabind::constructor<>()) \
		.def("at", &vector_T::at) \
		.def("front", &vector_T::front) \
		.def("back", &vector_T::back) \
		.def("push_back", &vector_T::push_back) \
		.def("pop_back", &vector_T::pop_back) \
		.def("erase", &vector_T::erase) \
		.def("clear", &vector_T::clear) \
		.def("empty", &vector_T::empty) \
		.def("size", &vector_T::size) \
		.def("resize", (void(vector_T::*)(unsigned int)) &vector_T::resize) \
		.def("resize", (void(vector_T::*)(unsigned int, const T&)) &vector_T::resize)

template <typename T>
class vector_wrapper
{
public:
	// Ctors
	vector_wrapper()
	{
	}
	vector_wrapper(const std::vector<T>& vector)
		: mVector(vector)
	{
	}
	vector_wrapper(std::vector<T>&& vector)
		: mVector(vector)
	{
	}

	T& at(unsigned int n)
	{
		return mVector.at(n);
	}
	T& front()
	{
		return mVector.front();
	}
	T& back()
	{
		return mVector.back();
	}

	void push_back(const T& val)
	{
		mVector.push_back(val);
	}
	void pop_back()
	{
		mVector.pop_back();
	}
	
	unsigned int erase(unsigned int n)
	{
		auto it = mVector.begin();
		std::advance(it, n);
		mVector.erase(it);
		return n;
	}
	void clear()
	{
		mVector.clear();
	}

	bool empty()
	{
		return mVector.empty();
	}
	unsigned int size()
	{
		return mVector.size();
	}
	void resize(unsigned int size)
	{
		mVector.resize(size);
	}
	void resize(unsigned int size, const T& val)
	{
		mVector.resize(size, val);
	}

	// Accesseurs
	std::vector<T>& GetVector()
	{
		return mVector;
	}
	const std::vector<T>& GetVector() const
	{
		return mVector;
	}

private:
	std::vector<T> mVector;
};

typedef vector_wrapper<b2Vec2> vector_b2Vec2;
