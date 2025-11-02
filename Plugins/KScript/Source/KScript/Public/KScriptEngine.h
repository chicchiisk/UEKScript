// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KScriptEngine.generated.h"

class UKScriptParser;
class UKScriptVariable;

/**
 * スクリプト実行命令の種類
 */
UENUM(BlueprintType)
enum class EKScriptCommandType : uint8
{
	Text,           // テキスト表示
	WaitClick,      // クリック待ち [l]
	WaitPageBreak,  // ページ区切り待ち [p]
	LineBreak,      // 改行 [r]
	ClearMessage,   // メッセージクリア [cm]
	Jump,           // ジャンプ [jump]
	Call,           // サブルーチン呼び出し [call]
	Return,         // サブルーチンから戻る [return]
	If,             // 条件分岐開始 [if]
	Else,           // 条件分岐のelse [else]
	EndIf,          // 条件分岐終了 [endif]
	Eval,           // 式の評価 [eval]
	Unknown         // 不明
};

/**
 * スクリプト実行命令
 */
USTRUCT(BlueprintType)
struct KSCRIPT_API FKScriptCommand
{
	GENERATED_BODY()

	UPROPERTY()
	EKScriptCommandType Type = EKScriptCommandType::Unknown;

	UPROPERTY()
	FString Text;

	UPROPERTY()
	TMap<FString, FString> Parameters;

	UPROPERTY()
	int32 LineNumber = 0;
};

/**
 * スクリプト実行状態
 */
UENUM(BlueprintType)
enum class EKScriptExecutionState : uint8
{
	Idle,           // アイドル状態
	Running,        // 実行中
	WaitingInput,   // 入力待ち
	Paused,         // 一時停止
	Finished        // 終了
};

/**
 * KScriptエンジン - スクリプトの実行を管理
 */
UCLASS(BlueprintType, Blueprintable)
class KSCRIPT_API UKScriptEngine : public UObject
{
	GENERATED_BODY()

public:
	UKScriptEngine();

	/**
	 * スクリプトをロードする
	 * @param ScriptText スクリプトテキスト
	 * @return ロード成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	bool LoadScript(const FString& ScriptText);

	/**
	 * スクリプトファイルをロードする
	 * @param FilePath スクリプトファイルパス
	 * @return ロード成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	bool LoadScriptFromFile(const FString& FilePath);

	/**
	 * スクリプト実行を開始する
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void Start();

	/**
	 * 次の命令を実行する
	 * @return まだ実行すべき命令がある場合true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	bool Step();

	/**
	 * 入力を受け取り、実行を継続する
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void OnInput();

	/**
	 * 実行を一時停止する
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void Pause();

	/**
	 * 実行を再開する
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void Resume();

	/**
	 * 実行を停止する
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	void Stop();

	/**
	 * 特定のラベルにジャンプする
	 * @param LabelName ラベル名
	 * @return ジャンプ成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript")
	bool JumpToLabel(const FString& LabelName);

	/**
	 * 現在の実行状態を取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript")
	EKScriptExecutionState GetExecutionState() const { return ExecutionState; }

	/**
	 * 現在の命令を取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript")
	const FKScriptCommand& GetCurrentCommand() const;

	/**
	 * パーサーを設定する
	 */
	void SetParser(UKScriptParser* InParser);

	/**
	 * 変数管理システムを設定する
	 */
	void SetVariableManager(UKScriptVariable* InVariableManager);

	// コマンドクラスから呼ばれるヘルパーメソッド

	/**
	 * 実行状態を設定
	 */
	void SetExecutionState(EKScriptExecutionState NewState) { ExecutionState = NewState; }

	/**
	 * 現在のコマンドインデックスを取得
	 */
	int32 GetCurrentCommandIndex() const { return CurrentCommandIndex; }

	/**
	 * 現在のコマンドインデックスを設定
	 */
	void SetCurrentCommandIndex(int32 NewIndex) { CurrentCommandIndex = NewIndex; }

	/**
	 * コールスタックにプッシュ
	 */
	void PushCallStack(int32 ReturnIndex) { CallStack.Add(ReturnIndex); }

	/**
	 * コールスタックからポップ
	 * @return 戻り先のインデックス（スタックが空の場合は-1）
	 */
	int32 PopCallStack() { return CallStack.Num() > 0 ? CallStack.Pop() : -1; }

	/**
	 * [else]または[endif]までスキップ
	 */
	void SkipToElseOrEndIf();

	/**
	 * [endif]までスキップ
	 */
	void SkipToEndIf();

	/**
	 * コマンド配列を取得
	 */
	const TArray<FKScriptCommand>& GetCommands() const { return Commands; }

protected:
	/**
	 * 命令を実行する
	 * @param Command 実行する命令
	 */
	virtual void ExecuteCommand(const FKScriptCommand& Command);

protected:
	UPROPERTY()
	TObjectPtr<UKScriptParser> Parser;

	UPROPERTY()
	TObjectPtr<UKScriptVariable> VariableManager;

	UPROPERTY()
	TObjectPtr<class UKScriptCommandFactory> CommandFactory;

	UPROPERTY()
	TArray<FKScriptCommand> Commands;

	UPROPERTY()
	int32 CurrentCommandIndex = 0;

	UPROPERTY()
	EKScriptExecutionState ExecutionState = EKScriptExecutionState::Idle;

	// ラベル名とコマンドインデックスのマップ
	UPROPERTY()
	TMap<FString, int32> Labels;

	// コールスタック（サブルーチン用）
	UPROPERTY()
	TArray<int32> CallStack;
};
