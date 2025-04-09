#include "pch.h"
#include "MaterialLibrary.h"


void SMatirialLibrary::Add(std::string MaterialTag, const std::shared_ptr<FMaterial>& InMaterial)
{
	MaterialTable[MaterialTag] = InMaterial;
}

const FMaterial& SMatirialLibrary::Get(std::string MaterialTag)
{
	return *MaterialTable[MaterialTag];
}
