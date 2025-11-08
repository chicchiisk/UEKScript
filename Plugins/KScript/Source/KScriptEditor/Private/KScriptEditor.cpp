// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptEditor.h"
#include "AssetTypeActions_KScriptAsset.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FKScriptEditorModule"

void FKScriptEditorModule::StartupModule()
{
	// アセットタイプアクションを登録
	RegisterAssetTypeActions();
}

void FKScriptEditorModule::ShutdownModule()
{
	// アセットタイプアクションを登録解除
	UnregisterAssetTypeActions();
}

void FKScriptEditorModule::RegisterAssetTypeActions()
{
	// AssetToolsモジュールを取得
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// KScriptアセット用のアセットタイプアクションを登録
	TSharedPtr<IAssetTypeActions> KScriptAssetTypeActions = MakeShareable(new FAssetTypeActions_KScriptAsset());
	AssetTools.RegisterAssetTypeActions(KScriptAssetTypeActions.ToSharedRef());
	RegisteredAssetTypeActions.Add(KScriptAssetTypeActions);
}

void FKScriptEditorModule::UnregisterAssetTypeActions()
{
	// AssetToolsモジュールがロード済みの場合のみ登録解除
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		// 登録したアセットタイプアクションを解除
		for (TSharedPtr<IAssetTypeActions> Action : RegisteredAssetTypeActions)
		{
			if (Action.IsValid())
			{
				AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
			}
		}
	}

	RegisteredAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKScriptEditorModule, KScriptEditor)