// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptWaitClickCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptWaitClickCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[クリック待ち]"));
	if (Engine)
	{
		Engine->SetExecutionState(EKScriptExecutionState::WaitingInput);
	}
	// TODO: UIにクリック待ちアイコンを表示
}
