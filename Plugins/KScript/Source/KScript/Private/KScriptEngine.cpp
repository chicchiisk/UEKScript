// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptEngine.h"
#include "KScript.h"
#include "KScriptParser.h"
#include "KScriptVariable.h"
#include "KScriptAsset.h"
#include "Commands/KScriptCommandFactory.h"
#include "Commands/KScriptCommandBase.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

UKScriptEngine::UKScriptEngine()
{
	ExecutionState = EKScriptExecutionState::Idle;
	CurrentCommandIndex = 0;

	// コマンドファクトリーを初期化
	CommandFactory = CreateDefaultSubobject<UKScriptCommandFactory>(TEXT("CommandFactory"));
}

bool UKScriptEngine::LoadScript(const FString& ScriptText)
{
	if (!Parser)
	{
		UE_LOG(LogKScript, Error, TEXT("パーサーが設定されていません"));
		return false;
	}

	// スクリプトを解析
	Commands.Empty();
	Labels.Empty();
	CallStack.Empty();
	CurrentCommandIndex = 0;
	ExecutionState = EKScriptExecutionState::Idle;

	if (Parser->Parse(ScriptText, Commands, Labels))
	{
		UE_LOG(LogKScript, Log, TEXT("スクリプトの読み込みに成功しました。コマンド数: %d, ラベル数: %d"),
			Commands.Num(), Labels.Num());
		return true;
	}

	UE_LOG(LogKScript, Error, TEXT("スクリプトの解析に失敗しました"));
	return false;
}

bool UKScriptEngine::LoadScriptFromFile(const FString& FilePath)
{
	FString ScriptText;
	if (!FFileHelper::LoadFileToString(ScriptText, *FilePath))
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトファイルの読み込みに失敗しました: %s"), *FilePath);
		return false;
	}

	UE_LOG(LogKScript, Log, TEXT("スクリプトファイルを読み込みました: %s"), *FilePath);
	return LoadScript(ScriptText);
}

bool UKScriptEngine::LoadScriptFromAsset(UKScriptAsset* ScriptAsset)
{
	if (!ScriptAsset)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトアセットがnullです"));
		return false;
	}

	FString ScriptText = ScriptAsset->GetScriptText();
	if (ScriptText.IsEmpty())
	{
		UE_LOG(LogKScript, Warning, TEXT("スクリプトアセット '%s' のテキストが空です"), *ScriptAsset->GetName());
	}

	UE_LOG(LogKScript, Log, TEXT("スクリプトアセットを読み込みました: %s"), *ScriptAsset->GetName());
	return LoadScript(ScriptText);
}

void UKScriptEngine::Start()
{
	if (Commands.Num() == 0)
	{
		UE_LOG(LogKScript, Warning, TEXT("実行するコマンドがありません"));
		return;
	}

	CurrentCommandIndex = 0;
	ExecutionState = EKScriptExecutionState::Running;
	UE_LOG(LogKScript, Log, TEXT("スクリプト実行を開始しました"));
}

bool UKScriptEngine::Step()
{
	if (ExecutionState == EKScriptExecutionState::Paused ||
		ExecutionState == EKScriptExecutionState::Finished)
	{
		return false;
	}

	if (CurrentCommandIndex >= Commands.Num())
	{
		ExecutionState = EKScriptExecutionState::Finished;
		UE_LOG(LogKScript, Log, TEXT("スクリプト実行が完了しました"));
		return false;
	}

	if (ExecutionState == EKScriptExecutionState::WaitingInput)
	{
		return false;
	}

	// 現在の命令を実行
	const FKScriptCommand& Command = Commands[CurrentCommandIndex];
	ExecuteCommand(Command);
	CurrentCommandIndex++;

	return CurrentCommandIndex < Commands.Num();
}

void UKScriptEngine::OnInput()
{
	if (ExecutionState == EKScriptExecutionState::WaitingInput)
	{
		ExecutionState = EKScriptExecutionState::Running;
		UE_LOG(LogKScript, Log, TEXT("入力を受け取りました。実行を再開します"));
	}
}

void UKScriptEngine::Pause()
{
	if (ExecutionState == EKScriptExecutionState::Running)
	{
		ExecutionState = EKScriptExecutionState::Paused;
		UE_LOG(LogKScript, Log, TEXT("実行を一時停止しました"));
	}
}

void UKScriptEngine::Resume()
{
	if (ExecutionState == EKScriptExecutionState::Paused)
	{
		ExecutionState = EKScriptExecutionState::Running;
		UE_LOG(LogKScript, Log, TEXT("実行を再開しました"));
	}
}

void UKScriptEngine::Stop()
{
	ExecutionState = EKScriptExecutionState::Idle;
	CurrentCommandIndex = 0;
	CallStack.Empty();
	UE_LOG(LogKScript, Log, TEXT("実行を停止しました"));
}

bool UKScriptEngine::JumpToLabel(const FString& LabelName)
{
	const int32* TargetIndex = Labels.Find(LabelName);
	if (TargetIndex)
	{
		CurrentCommandIndex = *TargetIndex;
		UE_LOG(LogKScript, Log, TEXT("ラベル '%s' にジャンプしました (インデックス: %d)"), *LabelName, *TargetIndex);
		return true;
	}

	UE_LOG(LogKScript, Warning, TEXT("ラベル '%s' が見つかりません"), *LabelName);
	return false;
}

const FKScriptCommand& UKScriptEngine::GetCurrentCommand() const
{
	static FKScriptCommand EmptyCommand;
	if (CurrentCommandIndex >= 0 && CurrentCommandIndex < Commands.Num())
	{
		return Commands[CurrentCommandIndex];
	}
	return EmptyCommand;
}

void UKScriptEngine::SetParser(UKScriptParser* InParser)
{
	Parser = InParser;
}

void UKScriptEngine::SetVariableManager(UKScriptVariable* InVariableManager)
{
	VariableManager = InVariableManager;
}

void UKScriptEngine::ExecuteCommand(const FKScriptCommand& Command)
{
	UE_LOG(LogKScript, Verbose, TEXT("コマンド実行: タイプ %d, 行番号 %d"),
		(int32)Command.Type, Command.LineNumber);

	// Commandパターン: コマンドファクトリーから適切なコマンドインスタンスを取得して実行
	if (CommandFactory)
	{
		UKScriptCommandBase* CommandInstance = CommandFactory->GetCommand(Command.Type);
		if (CommandInstance)
		{
			CommandInstance->Execute(this, Command, VariableManager);
		}
		else
		{
			UE_LOG(LogKScript, Warning, TEXT("コマンドタイプ %d のハンドラーが見つかりません"), (int32)Command.Type);
		}
	}
	else
	{
		UE_LOG(LogKScript, Error, TEXT("コマンドファクトリーが初期化されていません"));
	}
}

void UKScriptEngine::SkipToElseOrEndIf()
{
	int32 IfDepth = 1;
	while (CurrentCommandIndex < Commands.Num())
	{
		CurrentCommandIndex++;
		if (CurrentCommandIndex >= Commands.Num())
			break;

		const FKScriptCommand& NextCommand = Commands[CurrentCommandIndex];
		if (NextCommand.Type == EKScriptCommandType::If)
		{
			IfDepth++;
		}
		else if (NextCommand.Type == EKScriptCommandType::Else && IfDepth == 1)
		{
			// 対応するelseが見つかった
			break;
		}
		else if (NextCommand.Type == EKScriptCommandType::EndIf)
		{
			IfDepth--;
			if (IfDepth == 0)
			{
				// 対応するendifが見つかった
				break;
			}
		}
	}
}

void UKScriptEngine::SkipToEndIf()
{
	int32 IfDepth = 1;
	while (CurrentCommandIndex < Commands.Num())
	{
		CurrentCommandIndex++;
		if (CurrentCommandIndex >= Commands.Num())
			break;

		const FKScriptCommand& NextCommand = Commands[CurrentCommandIndex];
		if (NextCommand.Type == EKScriptCommandType::If)
		{
			IfDepth++;
		}
		else if (NextCommand.Type == EKScriptCommandType::EndIf)
		{
			IfDepth--;
			if (IfDepth == 0)
			{
				// 対応するendifが見つかった
				break;
			}
		}
	}
}
