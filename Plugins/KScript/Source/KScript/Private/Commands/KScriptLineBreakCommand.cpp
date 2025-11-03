// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptLineBreakCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptLineBreakCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[改行]"));
	// TODO: UIに改行を送る
}
