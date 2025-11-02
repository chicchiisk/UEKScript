// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptCommands.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

// テキスト表示コマンド
void UKScriptTextCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[テキスト] %s"), *Command.Text);
	// TODO: UIシステムと連携してテキストを表示
}

// クリック待ちコマンド
void UKScriptWaitClickCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[クリック待ち]"));
	if (Engine)
	{
		Engine->SetExecutionState(EKScriptExecutionState::WaitingInput);
	}
	// TODO: UIにクリック待ちアイコンを表示
}

// ページ区切り待ちコマンド
void UKScriptWaitPageBreakCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[ページ区切り待ち]"));
	if (Engine)
	{
		Engine->SetExecutionState(EKScriptExecutionState::WaitingInput);
	}
	// TODO: ページ区切り処理
}

// 改行コマンド
void UKScriptLineBreakCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[改行]"));
	// TODO: UIに改行を送る
}

// メッセージクリアコマンド
void UKScriptClearMessageCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[メッセージクリア]"));
	// TODO: UIのメッセージをクリア
}

// ジャンプコマンド
void UKScriptJumpCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* TargetLabel = Command.Parameters.Find(TEXT("target"));
	if (TargetLabel && Engine)
	{
		Engine->JumpToLabel(*TargetLabel);
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("ジャンプコマンドに 'target' パラメータがありません"));
	}
}

// 条件分岐コマンド
void UKScriptIfCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* Expression = Command.Parameters.Find(TEXT("exp"));
	if (Expression && VariableManager && Engine)
	{
		FKScriptVariableValue Result;
		if (VariableManager->EvaluateExpression(*Expression, Result))
		{
			bool Condition = Result.AsBool();
			UE_LOG(LogKScript, Log, TEXT("[条件分岐] %s = %s"), **Expression, Condition ? TEXT("真") : TEXT("偽"));

			// 条件が偽の場合、対応する[else]または[endif]までスキップ
			if (!Condition)
			{
				Engine->SkipToElseOrEndIf();
			}
		}
		else
		{
			UE_LOG(LogKScript, Warning, TEXT("条件式の評価に失敗しました: %s"), **Expression);
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("Ifコマンドに 'exp' パラメータまたは変数マネージャーがありません"));
	}
}

// Elseコマンド
void UKScriptElseCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	// elseに到達したということは、ifの条件が真だった場合
	// endifまでスキップする
	if (Engine)
	{
		Engine->SkipToEndIf();
	}
	UE_LOG(LogKScript, Log, TEXT("[Else] - endifまでスキップ"));
}

// EndIfコマンド
void UKScriptEndIfCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	// endifは何もしない（条件分岐の終了マーカー）
	UE_LOG(LogKScript, Log, TEXT("[EndIf]"));
}

// 式評価コマンド
void UKScriptEvalCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
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
				UE_LOG(LogKScript, Log, TEXT("[式評価] %s = %s"), *VarName, *Result.AsString());
			}
			else
			{
				UE_LOG(LogKScript, Warning, TEXT("式の評価に失敗しました: %s"), *ValueExpr);
			}
		}
		else
		{
			// 単純な式評価（代入なし）
			FKScriptVariableValue Result;
			if (VariableManager->EvaluateExpression(TrimmedExpr, Result))
			{
				UE_LOG(LogKScript, Log, TEXT("[式評価] 結果: %s"), *Result.AsString());
			}
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("Evalコマンドに 'exp' パラメータまたは変数マネージャーがありません"));
	}
}

// サブルーチン呼び出しコマンド
void UKScriptCallCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	const FString* TargetLabel = Command.Parameters.Find(TEXT("target"));
	if (TargetLabel && Engine)
	{
		// 現在の位置をコールスタックにプッシュ
		int32 CurrentIndex = Engine->GetCurrentCommandIndex();
		Engine->PushCallStack(CurrentIndex);
		UE_LOG(LogKScript, Log, TEXT("[サブルーチン呼び出し] %s (戻り先インデックス: %d)"), **TargetLabel, CurrentIndex);

		// ラベルにジャンプ
		if (!Engine->JumpToLabel(*TargetLabel))
		{
			// ジャンプに失敗した場合、スタックから削除
			Engine->PopCallStack();
		}
		else
		{
			// ジャンプ後、インデックスが進んでしまうので1つ戻す
			Engine->SetCurrentCommandIndex(Engine->GetCurrentCommandIndex() - 1);
		}
	}
	else
	{
		UE_LOG(LogKScript, Warning, TEXT("Callコマンドに 'target' パラメータがありません"));
	}
}

// サブルーチンから戻るコマンド
void UKScriptReturnCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	if (Engine)
	{
		int32 ReturnIndex = Engine->PopCallStack();
		if (ReturnIndex >= 0)
		{
			Engine->SetCurrentCommandIndex(ReturnIndex);
			UE_LOG(LogKScript, Log, TEXT("[Return] インデックス %d に戻ります"), ReturnIndex);
		}
		else
		{
			UE_LOG(LogKScript, Warning, TEXT("コールスタックが空の状態でReturnコマンドが呼ばれました"));
		}
	}
}
