// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptCommandFactory.h"
#include "KScript.h"
#include "Commands/KScriptTextCommand.h"
#include "Commands/KScriptWaitClickCommand.h"
#include "Commands/KScriptWaitPageBreakCommand.h"
#include "Commands/KScriptLineBreakCommand.h"
#include "Commands/KScriptClearMessageCommand.h"
#include "Commands/KScriptJumpCommand.h"
#include "Commands/KScriptIfCommand.h"
#include "Commands/KScriptElseCommand.h"
#include "Commands/KScriptEndIfCommand.h"
#include "Commands/KScriptEvalCommand.h"
#include "Commands/KScriptCallCommand.h"
#include "Commands/KScriptReturnCommand.h"
#include "Commands/KScriptBgCommand.h"
#include "Commands/KScriptCharaShowCommand.h"
#include "Commands/KScriptCharaHideCommand.h"
#include "Commands/KScriptPlayBgmCommand.h"
#include "Commands/KScriptStopBgmCommand.h"
#include "Commands/KScriptPlaySeCommand.h"
#include "Commands/KScriptSaveCommand.h"
#include "Commands/KScriptLoadCommand.h"
#include "Commands/KScriptTransCommand.h"

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

	// Phase2: マルチメディアコマンド
	RegisterCommand<UKScriptBgCommand>(EKScriptCommandType::Bg);
	RegisterCommand<UKScriptCharaShowCommand>(EKScriptCommandType::CharaShow);
	RegisterCommand<UKScriptCharaHideCommand>(EKScriptCommandType::CharaHide);
	RegisterCommand<UKScriptPlayBgmCommand>(EKScriptCommandType::PlayBgm);
	RegisterCommand<UKScriptStopBgmCommand>(EKScriptCommandType::StopBgm);
	RegisterCommand<UKScriptPlaySeCommand>(EKScriptCommandType::PlaySe);

	// Phase3: セーブ/ロード・トランジションコマンド
	RegisterCommand<UKScriptSaveCommand>(EKScriptCommandType::Save);
	RegisterCommand<UKScriptLoadCommand>(EKScriptCommandType::Load);
	RegisterCommand<UKScriptTransCommand>(EKScriptCommandType::Trans);
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
