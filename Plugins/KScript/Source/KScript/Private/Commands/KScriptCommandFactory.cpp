// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptCommandFactory.h"
#include "Commands/KScriptCommands.h"

UKScriptCommandFactory::UKScriptCommandFactory()
{
	RegisterAllCommands();
}

void UKScriptCommandFactory::RegisterAllCommands()
{
	RegisterCommand(EKScriptCommandType::Text, UKScriptTextCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::WaitClick, UKScriptWaitClickCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::WaitPageBreak, UKScriptWaitPageBreakCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::LineBreak, UKScriptLineBreakCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::ClearMessage, UKScriptClearMessageCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::Jump, UKScriptJumpCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::If, UKScriptIfCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::Else, UKScriptElseCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::EndIf, UKScriptEndIfCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::Eval, UKScriptEvalCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::Call, UKScriptCallCommand::StaticClass());
	RegisterCommand(EKScriptCommandType::Return, UKScriptReturnCommand::StaticClass());
}

void UKScriptCommandFactory::RegisterCommand(EKScriptCommandType CommandType, TSubclassOf<UKScriptCommandBase> CommandClass)
{
	if (CommandClass)
	{
		UKScriptCommandBase* CommandInstance = NewObject<UKScriptCommandBase>(this, CommandClass);
		CommandMap.Add(CommandType, CommandInstance);
	}
}

UKScriptCommandBase* UKScriptCommandFactory::GetCommand(EKScriptCommandType CommandType)
{
	TObjectPtr<UKScriptCommandBase>* Command = CommandMap.Find(CommandType);
	if (Command && *Command)
	{
		return *Command;
	}

	UE_LOG(LogTemp, Warning, TEXT("KScriptCommandFactory: No command registered for type %d"), (int32)CommandType);
	return nullptr;
}
