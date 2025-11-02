// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KScriptSubsystem.generated.h"

class UKScriptEngine;
class UKScriptParser;
class UKScriptVariable;

/**
 * KScriptサブシステム - ゲーム全体でのKScript管理
 * GameInstanceSubsystemとして実装し、ゲームインスタンス全体でKScriptを管理
 */
UCLASS()
class KSCRIPT_API UKScriptSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UKScriptSubsystem();

	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End of USubsystem interface

	/**
	 * スクリプトエンジンを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript")
	UKScriptEngine* GetScriptEngine() const { return ScriptEngine; }

	/**
	 * スクリプトパーサーを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript")
	UKScriptParser* GetScriptParser() const { return ScriptParser; }

	/**
	 * 変数管理システムを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript")
	UKScriptVariable* GetVariableManager() const { return VariableManager; }

	/**
	 * スクリプトファイルをロードして実行
	 * @param FilePath スクリプトファイルのパス
	 * @return ロード成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	bool LoadAndStartScript(const FString& FilePath);

	/**
	 * スクリプトテキストをロードして実行
	 * @param ScriptText スクリプトテキスト
	 * @return ロード成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	bool LoadAndStartScriptFromText(const FString& ScriptText);

	/**
	 * スクリプト実行を一時停止
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void PauseScript();

	/**
	 * スクリプト実行を再開
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void ResumeScript();

	/**
	 * スクリプト実行を停止
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void StopScript();

	/**
	 * ユーザー入力を処理（クリック待ちなどの継続）
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void HandleInput();

	/**
	 * スクリプトを1ステップ実行
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	bool StepScript();

protected:
	UPROPERTY()
	TObjectPtr<UKScriptEngine> ScriptEngine;

	UPROPERTY()
	TObjectPtr<UKScriptParser> ScriptParser;

	UPROPERTY()
	TObjectPtr<UKScriptVariable> VariableManager;
};
