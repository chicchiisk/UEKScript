// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptClearMessageCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptUIManager.h"
#include "KScriptVariable.h"

void UKScriptClearMessageCommand::Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager)
{
	UE_LOG(LogKScript, Log, TEXT("[メッセージクリア]"));
	// TODO: UIのメッセージをクリア
	UKScriptUIManager* UIManager = GetWorld()->GetSubsystem<UKScriptUIManager>();
	if (UIManager)
	{
		UIManager->ClearText();
	}
}
