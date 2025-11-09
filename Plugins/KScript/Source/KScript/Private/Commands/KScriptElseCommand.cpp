// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptElseCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptVariable.h"

void UKScriptElseCommand::Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager)
{
	// elseに到達したということは、ifの条件が真だった場合
	// endifまでスキップする
	if (Engine)
	{
		Engine->SkipToEndIf();
	}
	UE_LOG(LogKScript, Log, TEXT("[Else] - endifまでスキップ"));
}
