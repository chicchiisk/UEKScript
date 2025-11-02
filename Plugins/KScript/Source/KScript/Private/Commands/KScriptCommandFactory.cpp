// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptCommandFactory.h"
#include "KScript.h"
#include "Commands/KScriptCommands.h"

UKScriptCommandFactory::UKScriptCommandFactory()
{
	RegisterAllCommands();
}

void UKScriptCommandFactory::RegisterAllCommands()
{
	RegisterCommand<UKScriptTextCommand>(EKScriptCommandType::Text);
	RegisterCommand<UKScriptWaitClickCommand>(EKScriptCommandType::WaitClick);
	RegisterCommand<UKScriptWaitPageBreakCommand>(EKScriptCommandType::WaitPageBreak);
	RegisterCommand<UKScriptLineBreakCommand>(EKScriptCommandType::LineBreak);
	RegisterCommand<UKScriptClearMessageCommand>(EKScriptCommandType::ClearMessage);
	RegisterCommand<UKScriptJumpCommand>(EKScriptCommandType::Jump);
	RegisterCommand<UKScriptIfCommand>(EKScriptCommandType::If);
	RegisterCommand<UKScriptElseCommand>(EKScriptCommandType::Else);
	RegisterCommand<UKScriptEndIfCommand>(EKScriptCommandType::EndIf);
	RegisterCommand<UKScriptEvalCommand>(EKScriptCommandType::Eval);
	RegisterCommand<UKScriptCallCommand>(EKScriptCommandType::Call);
	RegisterCommand<UKScriptReturnCommand>(EKScriptCommandType::Return);
}


UKScriptCommandBase* UKScriptCommandFactory::GetCommand(EKScriptCommandType CommandType)
{
	TObjectPtr<UKScriptCommandBase>* Command = CommandMap.Find(CommandType);
	if (Command && *Command)
	{
		return *Command;
	}
	
	UE_LOG(LogKScript, Warning, TEXT("コマンドタイプ %d に対応するコマンドが登録されていません"), (int32)CommandType);
	return nullptr;
}
