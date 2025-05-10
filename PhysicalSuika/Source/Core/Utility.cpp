#include "pch.h"

#include "Debug.h"

#include <filesystem>
#include <fstream>

namespace Utility
{
	std::string LoadFileToString(const std::string& Path)
	{
		if (!std::filesystem::exists(Path))
		{
			Log::Log("File '" + Path + "' does not exist.");
			return {};
		}

		std::ifstream Ifs(Path, std::ios::in | std::ios::binary);
		if (!Ifs.is_open())
		{
			Log::Log("Could not open file '" + Path + "': " + std::strerror(errno));
			return {};
		}

		std::string TheFileString((std::istreambuf_iterator<char>(Ifs)), std::istreambuf_iterator<char>());
		return TheFileString;
	}
}
