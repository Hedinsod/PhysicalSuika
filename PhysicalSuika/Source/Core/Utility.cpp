#include "pch.h"

#include "Debug.h"

#include <fstream>

namespace Utility
{
    std::string LoadFileToString(const std::string& Path)
    {
		std::string TheFileString;

		std::ifstream Ifs(Path, std::ios::in | std::ios::binary);
		if (!Ifs.is_open())
		{
			Log::Log("Could not open file \'" + Path + "\'");
			return TheFileString;
		}

		Ifs.seekg(0, std::ios::end);
		size_t Length = Ifs.tellg();

		TheFileString.resize(Length);

		// reset position		
		Ifs.seekg(0, std::ios::beg);
		Ifs.read(&TheFileString[0], Length);
		Ifs.close();

		return TheFileString;
    }
}
