// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptWaitPageBreakCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"

void UKScriptWaitPageBreakCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[ページ区切り待ち]"));
	
	if (Engine)
	{
		// 入力待ち状態に設定（クリアは再開時にエンジン側で行う）
		Engine->SetExecutionState(EKScriptExecutionState::WaitingInput);
	}
}
