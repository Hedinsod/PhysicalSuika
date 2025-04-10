#include "pch.h"
#include <random>
#include <iostream>
#include <fstream>

namespace Utility
{
	int GetRandom(int Low, int Up)
	{
		static std::random_device dev; 
		static std::mt19937 rng;
		static std::uniform_int_distribution<std::mt19937::result_type> dist(0, 65565);

		int Correction = Low < 0 ? 1 : 0;
		int Random = dist(rng);

		return (Random % (Up - Low)) + Low + Correction;
	}

#if defined( _DEBUG )
	void AssertLog(const char* Condition, const char* FileName, int32_t LineNumber)
	{
		std::cerr << "Assertion failed: " << Condition 
		          << " in " << FileName
		          << ", at " << LineNumber << std::endl;
	}
#endif

	void Log(const std::string& Message)
	{
		std::cerr << Message << std::endl;
	}

    std::string LoadFileToString(const std::string& Path)
    {
		std::string TheFileString;

		std::ifstream Ifs(Path, std::ios::in | std::ios::binary);
		if (!Ifs.is_open())
		{
			Log("Could not open file \'" + Path + "\'");
			return TheFileString;  // Creating invalide shader
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
