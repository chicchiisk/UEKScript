// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptEvalCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

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
