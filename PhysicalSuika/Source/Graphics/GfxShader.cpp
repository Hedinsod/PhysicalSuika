#include "pch.h"
#include "GfxShader.h"


static EGfxShaderType GetShaderType(const std::string& Type)
{
	if (Type == "vertex")
	{
		return EGfxShaderType::Vertex;
	}
	else if (Type == "fragment" || Type == "pixel")
	{
		return EGfxShaderType::Pixel;
	}
	else
	{
		GAssertLog(false, "Unknown shader type");
		return EGfxShaderType::None;
	}
}

void SGfxShaderFactory::LoadSourceFromFile(const std::string& Path)
{
	std::string CombinedSource = Utility::LoadFileToString(Path);
	GAssert(CombinedSource.size() > 0);

	const char* TypeToken = "#type";
	size_t TypeTokenLen = strlen(TypeToken);
	size_t Pos = CombinedSource.find(TypeToken, 0);

	while (Pos != std::string::npos)
	{
		size_t EOL = CombinedSource.find_first_of("\r\n", Pos);

		size_t Begin = Pos + TypeTokenLen + 1;
		std::string Type = CombinedSource.substr(Begin, EOL - Begin);

		GAssert(Type == "vertex" || Type == "pixel" || Type == "fragment" /*, "Invalid shader type" */);

		size_t NextLinePos = CombinedSource.find_first_not_of("\r\n", EOL);
		Pos = CombinedSource.find(TypeToken, NextLinePos);

		AddSource(GetShaderType(Type), CombinedSource.substr(NextLinePos, Pos - (NextLinePos == std::string::npos ? CombinedSource.size() - 1 : NextLinePos)));
	}
}

SGfxShaderFactory::~SGfxShaderFactory()
{
}

void SGfxShaderFactory::AddSource(EGfxShaderType Type, const std::string& Source)
{
	Sources.push_back({Type, Source});
}
