#pragma once
#include "Includes.hpp"

struct FileInfo
{
	std::string localFilePath;
	std::string fileName;
	std::string fileStem;
	std::string fileExtension;
};

std::vector<FileInfo> getFiles(const std::string path, const std::string filter = "*.*", bool recursively = false);
