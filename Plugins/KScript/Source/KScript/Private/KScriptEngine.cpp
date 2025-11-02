// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptEngine.h"
#include "KScriptParser.h"
#include "KScriptVariable.h"
#include "Commands/KScriptCommandFactory.h"
#include "Commands/KScriptCommandBase.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

UKScriptEngine::UKScriptEngine()
{
	ExecutionState = EKScriptExecutionState::Idle;
	CurrentCommandIndex = 0;

	// コマンドファクトリーを初期化
	CommandFactory = NewObject<UKScriptCommandFactory>(this);
}

bool UKScriptEngine::LoadScript(const FString& ScriptText)
{
	if (!Parser)
	{
		UE_LOG(LogTemp, Error, TEXT("KScriptEngine: Parser is not set"));
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
		UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Script loaded successfully. %d commands, %d labels"),
			Commands.Num(), Labels.Num());
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("KScriptEngine: Failed to parse script"));
	return false;
}

bool UKScriptEngine::LoadScriptFromFile(const FString& FilePath)
{
	FString ScriptText;
	if (!FFileHelper::LoadFileToString(ScriptText, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("KScriptEngine: Failed to load script file: %s"), *FilePath);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Loaded script file: %s"), *FilePath);
	return LoadScript(ScriptText);
}

void UKScriptEngine::Start()
{
	if (Commands.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: No commands to execute"));
		return;
	}

	CurrentCommandIndex = 0;
	ExecutionState = EKScriptExecutionState::Running;
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Started execution"));
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
		UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Execution finished"));
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
		UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Input received, resuming execution"));
	}
}

void UKScriptEngine::Pause()
{
	if (ExecutionState == EKScriptExecutionState::Running)
	{
		ExecutionState = EKScriptExecutionState::Paused;
		UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Execution paused"));
	}
}

void UKScriptEngine::Resume()
{
	if (ExecutionState == EKScriptExecutionState::Paused)
	{
		ExecutionState = EKScriptExecutionState::Running;
		UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Execution resumed"));
	}
}

void UKScriptEngine::Stop()
{
	ExecutionState = EKScriptExecutionState::Idle;
	CurrentCommandIndex = 0;
	CallStack.Empty();
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Execution stopped"));
}

bool UKScriptEngine::JumpToLabel(const FString& LabelName)
{
	const int32* TargetIndex = Labels.Find(LabelName);
	if (TargetIndex)
	{
		CurrentCommandIndex = *TargetIndex;
		UE_LOG(LogTemp, Log, TEXT("KScriptEngine: Jumped to label '%s' (index %d)"), *LabelName, *TargetIndex);
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: Label '%s' not found"), *LabelName);
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
	UE_LOG(LogTemp, Verbose, TEXT("KScriptEngine: Executing command type %d at line %d"),
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
			UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: No handler for command type %d"), (int32)Command.Type);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("KScriptEngine: CommandFactory is not initialized"));
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
