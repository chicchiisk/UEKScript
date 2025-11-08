// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IAssetTypeActions;

class FKScriptEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** 登録されたアセットタイプアクション */
	TArray<TSharedPtr<IAssetTypeActions>> RegisteredAssetTypeActions;

	/** アセットタイプアクションを登録 */
	void RegisterAssetTypeActions();

	/** アセットタイプアクションを登録解除 */
	void UnregisterAssetTypeActions();
};
