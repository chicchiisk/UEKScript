// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptClearMessageCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptClearMessageCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[メッセージクリア]"));
	// TODO: UIのメッセージをクリア
}
