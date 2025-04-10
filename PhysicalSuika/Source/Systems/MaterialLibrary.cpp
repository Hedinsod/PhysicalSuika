#include "pch.h"
#include "MaterialLibrary.h"


void SMatirialLibrary::Add(std::string MaterialTag, const StdShared<FMaterial>& InMaterial)
{
	MaterialTable[MaterialTag] = InMaterial;
}

const FMaterial& SMatirialLibrary::Get(std::string MaterialTag)
{
	return *MaterialTable[MaterialTag];
}
