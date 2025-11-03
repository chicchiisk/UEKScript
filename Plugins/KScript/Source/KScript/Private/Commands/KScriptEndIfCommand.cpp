// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptEndIfCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptEndIfCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	// endifは何もしない（条件分岐の終了マーカー）
	UE_LOG(LogKScript, Log, TEXT("[EndIf]"));
}
