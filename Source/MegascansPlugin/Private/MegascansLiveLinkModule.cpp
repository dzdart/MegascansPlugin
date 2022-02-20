// Copyright Epic Games, Inc. All Rights Reserved.
#include "IMegascansLiveLinkModule.h"
#include "UI/QMSUIManager.h"
#include "UI/MaterialBlendingDetails.h"
#include "UI/MSSettings.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/Paths.h"



#define LOCTEXT_NAMESPACE "MegascansPlugin"

class FQMSLiveLinkModule : public IMegascansLiveLinkModule
{
public:
	virtual void StartupModule() override
	{
		FQMSLiveLinkModule::CopyMat();
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
	}

	void CopyMat() {
		//复制MSP目录到项目的MSP目录下
		const FString bridge_dir = L"MegascansPlugin/Content/MSPresets";
		const FString engine_plugin_content = FPaths::Combine(FPaths::EnginePluginsDir(),bridge_dir);
		const FString project_plugin_content = FPaths::Combine(FPaths::ProjectPluginsDir(),bridge_dir);
		const FString project_MSP = FPaths::Combine(FPaths::ProjectContentDir(),L"MSPresets");
		//IPlatformFile::CopyDirectoryTree();
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (FPaths::DirectoryExists(engine_plugin_content))
		{
			PlatformFile.CopyDirectoryTree(*project_MSP, *engine_plugin_content,true);
		}
		else if (FPaths::DirectoryExists(project_plugin_content))
		{
			PlatformFile.CopyDirectoryTree(*project_MSP, *project_plugin_content, true);
		}
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
