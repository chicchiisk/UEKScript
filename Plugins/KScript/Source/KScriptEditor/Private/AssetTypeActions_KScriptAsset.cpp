// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_KScriptAsset.h"
#include "KScriptAsset.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformApplicationMisc.h"
#include "DesktopPlatformModule.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_KScriptAsset"

FText FAssetTypeActions_KScriptAsset::GetName() const
{
	return LOCTEXT("FAssetTypeActions_KScriptAsset", "KScript");
}

FColor FAssetTypeActions_KScriptAsset::GetTypeColor() const
{
	// アセットの色（エディタで表示される色）
	return FColor(255, 196, 128); // オレンジ系
}

UClass* FAssetTypeActions_KScriptAsset::GetSupportedClass() const
{
	return UKScriptAsset::StaticClass();
}

uint32 FAssetTypeActions_KScriptAsset::GetCategories()
{
	// "Misc"カテゴリに表示
	return EAssetTypeCategories::Misc;
}

void FAssetTypeActions_KScriptAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	// 簡易的なエディタ実装：外部エディタで開く
	for (UObject* Obj : InObjects)
	{
		UKScriptAsset* Asset = Cast<UKScriptAsset>(Obj);
		if (Asset)
		{
			// 一時ファイルに書き出して、システムのデフォルトエディタで開く
			FString TempFilePath = FPaths::CreateTempFilename(*FPaths::ProjectSavedDir(), TEXT("KScript_"), TEXT(".ks"));

			if (FFileHelper::SaveStringToFile(Asset->GetScriptText(), *TempFilePath))
			{
				FPlatformProcess::LaunchFileInDefaultExternalApplication(*TempFilePath, nullptr, ELaunchVerb::Edit);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
