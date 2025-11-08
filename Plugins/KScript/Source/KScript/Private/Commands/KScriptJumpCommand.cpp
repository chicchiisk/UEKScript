// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptJumpCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

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
