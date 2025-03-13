
#include "NinjaNameGenerator.h"

NinjaNameGenerator::NinjaNameGenerator(const std::string& firstName, const std::string& lastName) 
	: m_firstName(firstName), m_lastName(lastName)
{
}

std::string NinjaNameGenerator::getNinjaName() const
{
	return getFirstNinjaName() + " " + getLastNinjaName();
}

std::string NinjaNameGenerator::getFirstNinjaName() const
{
	std::string firstNinjaName = "";
	for (int i = 0; i < 4; i++)
	{
		firstNinjaName += m_abc.find(m_firstName[i])->second;
	}
	return firstNinjaName;
}

std::string NinjaNameGenerator::getLastNinjaName() const
{
	std::string lastNinjaName = "";
	for (int i = 0; i < 3; i++)
	{
		lastNinjaName += m_abc.find(m_lastName[i])->second;
	}
	return lastNinjaName;
}
