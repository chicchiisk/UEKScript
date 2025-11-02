// Copyright Koromosoft. All Rights Reserved.

#include "KScriptEngine.h"
#include "KScriptParser.h"
#include "KScriptVariable.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

UKScriptEngine::UKScriptEngine()
{
	ExecutionState = EKScriptExecutionState::Idle;
	CurrentCommandIndex = 0;
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

	switch (Command.Type)
	{
	case EKScriptCommandType::Text:
		ExecuteTextCommand(Command);
		break;
	case EKScriptCommandType::WaitClick:
		ExecuteWaitClickCommand(Command);
		break;
	case EKScriptCommandType::WaitPageBreak:
		ExecuteWaitPageBreakCommand(Command);
		break;
	case EKScriptCommandType::LineBreak:
		ExecuteLineBreakCommand(Command);
		break;
	case EKScriptCommandType::ClearMessage:
		ExecuteClearMessageCommand(Command);
		break;
	case EKScriptCommandType::Jump:
		ExecuteJumpCommand(Command);
		break;
	case EKScriptCommandType::If:
		ExecuteIfCommand(Command);
		break;
	case EKScriptCommandType::Else:
		ExecuteElseCommand(Command);
		break;
	case EKScriptCommandType::EndIf:
		ExecuteEndIfCommand(Command);
		break;
	case EKScriptCommandType::Eval:
		ExecuteEvalCommand(Command);
		break;
	case EKScriptCommandType::Call:
		ExecuteCallCommand(Command);
		break;
	case EKScriptCommandType::Return:
		ExecuteReturnCommand(Command);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: Unknown command type"));
		break;
	}
}

void UKScriptEngine::ExecuteTextCommand(const FKScriptCommand& Command)
{
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [Text] %s"), *Command.Text);
	// TODO: UIシステムと連携してテキストを表示
}

void UKScriptEngine::ExecuteWaitClickCommand(const FKScriptCommand& Command)
{
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [WaitClick]"));
	ExecutionState = EKScriptExecutionState::WaitingInput;
	// TODO: UIにクリック待ちアイコンを表示
}

void UKScriptEngine::ExecuteWaitPageBreakCommand(const FKScriptCommand& Command)
{
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [WaitPageBreak]"));
	ExecutionState = EKScriptExecutionState::WaitingInput;
	// TODO: ページ区切り処理
}

void UKScriptEngine::ExecuteLineBreakCommand(const FKScriptCommand& Command)
{
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [LineBreak]"));
	// TODO: UIに改行を送る
}

void UKScriptEngine::ExecuteClearMessageCommand(const FKScriptCommand& Command)
{
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [ClearMessage]"));
	// TODO: UIのメッセージをクリア
}

void UKScriptEngine::ExecuteJumpCommand(const FKScriptCommand& Command)
{
	const FString* TargetLabel = Command.Parameters.Find(TEXT("target"));
	if (TargetLabel)
	{
		JumpToLabel(*TargetLabel);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: Jump command missing 'target' parameter"));
	}
}

void UKScriptEngine::ExecuteIfCommand(const FKScriptCommand& Command)
{
	const FString* Expression = Command.Parameters.Find(TEXT("exp"));
	if (Expression && VariableManager)
	{
		FKScriptVariableValue Result;
		if (VariableManager->EvaluateExpression(*Expression, Result))
		{
			bool Condition = Result.AsBool();
			UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [If] %s = %s"), **Expression, Condition ? TEXT("true") : TEXT("false"));

			// 条件が偽の場合、対応する[else]または[endif]までスキップ
			if (!Condition)
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
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: Failed to evaluate if condition: %s"), **Expression);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: If command missing 'exp' parameter or VariableManager"));
	}
}

void UKScriptEngine::ExecuteElseCommand(const FKScriptCommand& Command)
{
	// elseに到達したということは、ifの条件が真だった場合
	// endifまでスキップする
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
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [Else] - Skipped to endif"));
}

void UKScriptEngine::ExecuteEndIfCommand(const FKScriptCommand& Command)
{
	// endifは何もしない（条件分岐の終了マーカー）
	UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [EndIf]"));
}

void UKScriptEngine::ExecuteEvalCommand(const FKScriptCommand& Command)
{
	const FString* Expression = Command.Parameters.Find(TEXT("exp"));
	if (Expression && VariableManager)
	{
		// 式を評価
		// evalタグは代入式を想定（例: f.hp=100, f.name="太郎"）
		FString TrimmedExpr = Expression->TrimStartAndEnd();
		int32 EqualPos = INDEX_NONE;
		if (TrimmedExpr.FindChar(TEXT('='), EqualPos))
		{
			FString VarName = TrimmedExpr.Left(EqualPos).TrimStartAndEnd();
			FString ValueExpr = TrimmedExpr.Mid(EqualPos + 1).TrimStartAndEnd();

			FKScriptVariableValue Result;
			if (VariableManager->EvaluateExpression(ValueExpr, Result))
			{
				VariableManager->SetVariableValue(VarName, Result);
				UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [Eval] %s = %s"), *VarName, *Result.AsString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: Failed to evaluate expression: %s"), *ValueExpr);
			}
		}
		else
		{
			// 単純な式評価（代入なし）
			FKScriptVariableValue Result;
			if (VariableManager->EvaluateExpression(TrimmedExpr, Result))
			{
				UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [Eval] Expression result: %s"), *Result.AsString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: Eval command missing 'exp' parameter or VariableManager"));
	}
}

void UKScriptEngine::ExecuteCallCommand(const FKScriptCommand& Command)
{
	const FString* TargetLabel = Command.Parameters.Find(TEXT("target"));
	if (TargetLabel)
	{
		// 現在の位置をコールスタックにプッシュ
		CallStack.Add(CurrentCommandIndex);
		UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [Call] %s (return to index %d)"), **TargetLabel, CurrentCommandIndex);

		// ラベルにジャンプ
		if (!JumpToLabel(*TargetLabel))
		{
			// ジャンプに失敗した場合、スタックから削除
			CallStack.Pop();
		}
		else
		{
			// ジャンプ後、インデックスが進んでしまうので1つ戻す
			CurrentCommandIndex--;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: Call command missing 'target' parameter"));
	}
}

void UKScriptEngine::ExecuteReturnCommand(const FKScriptCommand& Command)
{
	if (CallStack.Num() > 0)
	{
		// コールスタックから戻り先を取得
		int32 ReturnIndex = CallStack.Pop();
		CurrentCommandIndex = ReturnIndex;
		UE_LOG(LogTemp, Log, TEXT("KScriptEngine: [Return] to index %d"), ReturnIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("KScriptEngine: Return command called with empty call stack"));
	}
}
