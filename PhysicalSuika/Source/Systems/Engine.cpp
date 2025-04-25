#include "pch.h"
#include "Engine.h"
#include "Core/Application.h"
#include "Graphics/GfxWindow.h"
#include "Graphics/Graphics.h"

Engine* Engine::Instance = nullptr;

void Engine::Init()
{
	Instance = new Engine;

	const FSettings& Settings = GApp->GetSettings();

	const float Frametime = 1.0f / Settings.TargetFPS;
	Instance->PhyScene = MakeScoped<SPhyScene>(Frametime, Settings.PhysicsSubStepsCount);
	GAssert(Instance->PhyScene);

	Instance->Graphics = MakeScoped<SRenderer>();
	GAssert(Instance->Graphics);

	Instance->LoadMaterials();
}

void Engine::Shutdown()
{
	delete Instance;
	Instance = nullptr;
}

void Engine::LoadMaterials()
{
	constexpr uint32_t TextureFormatRGBA = 0x1908;

	StdScoped<SGfxTextureFactory> TexFactory = SGraphics::GetTextureFactory();

	uint32_t WhiteTexture = 0xffffffff;
	StdShared<SGfxTexture> WhiteTex = TexFactory->CreateFromData(&WhiteTexture, 1, 1, TextureFormatRGBA);
	StdShared<SGfxTexture> BerryTex = TexFactory->LoadFromFile("Content/Textures/Tamoto.png");

	MatirialLibrary.Add("Default", MakeShared<FMaterial>(/*.Density*/ 0.0f, /*.Friction*/ 0.0f, /*.GravityScale*/ 0.0f, /*Color*/glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), WhiteTex));

	MatirialLibrary.Add("Berry", MakeShared<FMaterial>(/*.Density*/ 0.0955f, /*.Friction*/ 0.2f, /*.GravityScale*/ 1.0f, /*Color*/glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), BerryTex));
	MatirialLibrary.Add("Glass", MakeShared<FMaterial>(/*.Density*/    0.0f, /*.Friction*/ 0.4f, /*.GravityScale*/ 1.0f, /*Color*/glm::vec4(0.0f, 0.5f, 0.8f, 1.0f), WhiteTex));

	MatirialLibrary.Add("Overlay", MakeShared<FMaterial>(/*.Density*/ 0.0f, /*.Friction*/ 0.0f, /*.GravityScale*/ 0.0f, /*Color*/glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), WhiteTex));
}
