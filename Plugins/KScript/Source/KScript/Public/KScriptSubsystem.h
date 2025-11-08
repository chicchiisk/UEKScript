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
	 * KScriptアセットからスクリプトをロードして実行
	 * @param ScriptAsset KScriptアセット
	 * @return ロード成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript", meta = (DisplayName = "Load And Start Script (Asset)"))
	bool LoadAndStartScriptFromAsset(class UKScriptAsset* ScriptAsset);

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

	// ========================================
	// セーブ/ロード機能
	// ========================================

	/**
	 * 現在のゲーム状態をセーブする
	 * @param SlotName セーブスロット名
	 * @param UserIndex ユーザーインデックス
	 * @return 成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|SaveLoad")
	bool SaveGame(const FString& SlotName, int32 UserIndex = 0);

	/**
	 * セーブデータをロードする
	 * @param SlotName セーブスロット名
	 * @param UserIndex ユーザーインデックス
	 * @return 成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|SaveLoad")
	bool LoadGame(const FString& SlotName, int32 UserIndex = 0);

	/**
	 * セーブデータが存在するかチェック
	 * @param SlotName セーブスロット名
	 * @param UserIndex ユーザーインデックス
	 * @return 存在する場合true
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|SaveLoad")
	bool DoesSaveGameExist(const FString& SlotName, int32 UserIndex = 0) const;

	/**
	 * セーブデータを削除
	 * @param SlotName セーブスロット名
	 * @param UserIndex ユーザーインデックス
	 * @return 成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|SaveLoad")
	bool DeleteSaveGame(const FString& SlotName, int32 UserIndex = 0);

	/**
	 * 現在実行中のスクリプトアセットパスを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript")
	FString GetCurrentScriptAssetPath() const { return CurrentScriptAssetPath; }

protected:
	/**
	 * 現在の状態をセーブゲームオブジェクトに保存
	 */
	void CaptureCurrentState(class UKScriptSaveGame* SaveGameObject);

	/**
	 * セーブゲームオブジェクトから状態を復元
	 */
	bool RestoreFromSaveGame(class UKScriptSaveGame* SaveGameObject);

protected:
	UPROPERTY()
	TObjectPtr<UKScriptEngine> ScriptEngine;

	UPROPERTY()
	TObjectPtr<UKScriptParser> ScriptParser;

	UPROPERTY()
	TObjectPtr<UKScriptVariable> VariableManager;

	/** 現在実行中のスクリプトアセットのパス */
	UPROPERTY()
	FString CurrentScriptAssetPath;
};
