#ifndef NINJANAMEGENERATOR_H
#define NINJANAMEGENERATOR_H

#include <iostream>
#include <string>
#include <map>

class NinjaNameGenerator final
{
public:
	NinjaNameGenerator(const std::string& firstName, const std::string& lastName);

	std::string getNinjaName() const;

private:

	std::string getFirstNinjaName() const;
	std::string getLastNinjaName() const;

	std::string m_firstName;
	std::string m_lastName;

	std::map<char, std::string> m_abc = {	{'a', "ka"}, {'b', "zu"}, {'c', "mu"}, {'d', "te"}, {'e', "ku"}, {'f', "lu"}, {'g', "ji"}, {'h', "ri"},
											{'i', "ki"}, {'j', "zu"}, {'k', "me"}, {'l', "ta"}, {'m', "rin"}, {'n', "to"}, {'o', "mo"}, {'p', "no"},
											{'q', "ke"}, {'r', "shi"}, {'s', "ari"}, {'t', "chi"}, {'u', "do"}, {'v', "ru"}, {'w', "mei"}, {'x', "na"},
											{'y', "fu"}, {'z', "zi"} };
};

#endif // NINJANAMEGENERATOR_H