// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_KScriptAsset.h"
#include "KScriptAsset.h"
#include "KScriptViewer.h"

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
	// KScriptViewerウィンドウを開く
	for (UObject* Obj : InObjects)
	{
		UKScriptAsset* Asset = Cast<UKScriptAsset>(Obj);
		if (Asset)
		{
			SKScriptViewer::OpenViewer(Asset);
		}
	}
}

#undef LOCTEXT_NAMESPACE
