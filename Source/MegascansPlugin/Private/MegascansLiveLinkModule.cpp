// Copyright Epic Games, Inc. All Rights Reserved.
#include "IMegascansLiveLinkModule.h"
#include "UI/QMSUIManager.h"
#include "UI/MaterialBlendingDetails.h"
#include "UI/MSSettings.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "LevelEditor.h"
#include "Modules/ModuleManager.h"



#define LOCTEXT_NAMESPACE "MegascansPlugin"

class FQMSLiveLinkModule : public IMegascansLiveLinkModule
{
public:
	virtual void StartupModule() override
	{
		
		if (GIsEditor && !IsRunningCommandlet())
		{
			 FQMSUIManager::Initialize();
		}

		auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout(
			"MaterialBlendSettings",
			FOnGetDetailCustomizationInstance::CreateStatic(&BlendSettingsCustomization::MakeInstance)
		);
		PropertyModule.NotifyCustomizationModuleChanged();
		FQMSLiveLinkModule::CopyMat();
	}




	void CopyMat() {
		//复制MSP目录到项目的MSP目录下
		const FString project_MSP = FPaths::Combine(FPaths::ProjectContentDir(), L"MSPresets");
		FString pluginpath= FModuleManager::Get().GetModuleFilename("MegascansPlugin");
		pluginpath = pluginpath.Replace(*FString("Binaries/Win64/UnrealEditor-MegascansPlugin.dll"),*FString("Content/MSPresets"));
		FPlatformFileManager::Get().GetPlatformFile().CopyDirectoryTree(*project_MSP,*pluginpath,true);
	}

	virtual void ShutdownModule() override
	{
		if ((GIsEditor && !IsRunningCommandlet()))
		{
			
		}

		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

			PropertyModule.UnregisterCustomClassLayout("MaterialBlendSettings");
		}
	}


};

IMPLEMENT_MODULE(FQMSLiveLinkModule, MegascansPlugin);

#undef LOCTEXT_NAMESPACE
