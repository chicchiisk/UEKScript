// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptAssetFactory.h"
#include "KScriptAsset.h"
#include "Misc/FileHelper.h"
#include "EditorFramework/AssetImportData.h"

UKScriptAssetFactory::UKScriptAssetFactory()
{
	// このファクトリがサポートするフォーマット
	Formats.Add(TEXT("ks;KScript File"));

	SupportedClass = UKScriptAsset::StaticClass();

	bCreateNew = false;
	bEditAfterNew = true;
	bEditorImport = true;
	bText = true;
}

UObject* UKScriptAssetFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd,
	FFeedbackContext* Warn)
{
	// 新しいKScriptAssetを作成
	UKScriptAsset* NewAsset = NewObject<UKScriptAsset>(InParent, InClass, InName, Flags);

	if (NewAsset)
	{
		// バッファからテキストを読み込み
		FString ScriptText(BufferEnd - Buffer, Buffer);
		NewAsset->SetScriptText(ScriptText);

		// ソースファイルパスを記録（存在する場合）
		if (CurrentFilename.Len() > 0)
		{
			NewAsset->SourceFilePath = CurrentFilename;
		}
	}

	return NewAsset;
}

bool UKScriptAssetFactory::DoesSupportClass(UClass* Class)
{
	return Class == UKScriptAsset::StaticClass();
}

UClass* UKScriptAssetFactory::ResolveSupportedClass()
{
	return UKScriptAsset::StaticClass();
}

bool UKScriptAssetFactory::FactoryCanImport(const FString& Filename)
{
	// .ksファイルのみをインポート可能
	return FPaths::GetExtension(Filename).Equals(TEXT("ks"), ESearchCase::IgnoreCase);
}
