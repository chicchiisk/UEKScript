// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptIfCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptIfCommand::Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager)
{
	const FString* Expression = Command->Parameters.Find(TEXT("exp"));
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
