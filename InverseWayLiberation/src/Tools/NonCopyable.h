#pragma once

class NonCopyable
{
protected:
	// Ctor & Dtor (les classes filles peuvent se [cons/dé]truire)
	NonCopyable() {}
	~NonCopyable() {}

private:
	// La copie est interdite
	NonCopyable(const NonCopyable& origin);
	NonCopyable& operator= (const NonCopyable& origin);
};