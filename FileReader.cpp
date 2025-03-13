#include "FileReader.h"

constexpr int FILE_NOT_FOUND = 1;

FileReader::FileReader(const std::string& path)
{
	m_fileStream = std::ifstream(path);
	if (m_fileStream.fail())
	{
		std::cerr << "File does not exist..." << std::endl;
		exit(FILE_NOT_FOUND);
	}
}

void FileReader::ReadFile(std::vector<std::string>& ninjaMap)
{
	std::string line = "";
	while (std::getline(m_fileStream, line))
	{
		ninjaMap.push_back(line);
	}
}

FileReader::~FileReader()
{
	m_fileStream.close();
}
