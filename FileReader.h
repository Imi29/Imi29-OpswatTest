#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class FileReader
{
public:
	FileReader(const std::string& path);

	void ReadFile(std::vector<std::string>& ninjaMap);

	~FileReader();
private:
	std::ifstream m_fileStream;
};

#endif // !FILEREADER_H