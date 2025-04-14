#pragma once

namespace Utility
{
	// Reads file into std::string and returns it
	// If fails to open file prints in log and returns empty string
	std::string LoadFileToString(const std::string& Path);
}

