// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptReturnCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptReturnCommand::Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager)
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
