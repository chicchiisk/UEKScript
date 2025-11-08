// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "KScriptAsset.generated.h"

/**
 * KScriptアセット - .ksスクリプトファイルを保持するアセット
 */
UCLASS(BlueprintType)
class KSCRIPT_API UKScriptAsset : public UObject
{
	GENERATED_BODY()

public:
	/** スクリプトのソーステキスト */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KScript", meta = (MultiLine = true))
	FString ScriptText;

	/** アセットのインポート時のファイルパス */
	UPROPERTY(VisibleAnywhere, Category = "KScript")
	FString SourceFilePath;

	/** スクリプトテキストを取得 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	FString GetScriptText() const { return ScriptText; }

	/** スクリプトテキストを設定 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void SetScriptText(const FString& InText) { ScriptText = InText; }
};
