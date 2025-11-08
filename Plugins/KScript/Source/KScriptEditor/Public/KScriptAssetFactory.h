// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "KScriptAssetFactory.generated.h"

/**
 * .ksファイルをインポートするファクトリクラス
 */
UCLASS()
class KSCRIPTEDITOR_API UKScriptAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UKScriptAssetFactory();

	// UFactory interface
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
		UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd,
		FFeedbackContext* Warn) override;
	virtual bool DoesSupportClass(UClass* Class) override;
	virtual UClass* ResolveSupportedClass() override;
	virtual bool FactoryCanImport(const FString& Filename) override;
	// End of UFactory interface
};
