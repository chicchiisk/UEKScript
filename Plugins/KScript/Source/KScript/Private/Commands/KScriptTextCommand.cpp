// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptTextCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptTextCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[テキスト] %s"), *Command.Text);
	// TODO: UIシステムと連携してテキストを表示
}
