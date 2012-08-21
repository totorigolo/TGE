#pragma once
#include <ostream>
#include <string>
#include <fstream>
#include <cassert>
#include <map>
#include <SFML/Graphics.hpp>

class ConfigReader
{
public:
	// Ctor & Dtor
	ConfigReader(const std::string& path);
	virtual ~ConfigReader(void);

	// Accesseurs
	bool IsOk()	{ return mIsOk; }
	bool SectionExists			(const std::string& name) const;
	int GetInt					(const std::string& section, const std::string& name, int default = 0) const;
	bool GetBool				(const std::string& section, const std::string& name, bool default = false) const;
	float GetFloat				(const std::string& section, const std::string& name, float default = 0.f) const;
	sf::Uint32 GetUint32		(const std::string& section, const std::string& name, const sf::Uint32 default = sf::Uint32()) const;
	const sf::Color GetColor	(const std::string& section, const std::string& name, const sf::Color& default = sf::Color()) const;
	const std::string GetString (const std::string& section, const std::string& name, const std::string& default = std::string()) const;

private:
	bool Process();
	const std::string ParseLine(const std::string& line, const std::string& section, int lineNumber);
	bool StoreNameValue(const std::string& section, const std::string& name, const std::string& value);

	bool mIsOk;
	std::map<std::string, std::map<std::string, std::string>*> mSections;
	std::string mPath;
	std::fstream* mFile;
};