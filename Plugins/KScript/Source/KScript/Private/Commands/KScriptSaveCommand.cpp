// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptSaveCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptSubsystem.h"
#include "Engine/GameInstance.h"

void UKScriptSaveCommand::Execute(const FKScriptCommand& Command, UKScriptEngine* Engine)
{
	if (!Engine)
	{
		return;
	}

	// スロット名を取得（デフォルトは"QuickSave"）
	FString SlotName = TEXT("QuickSave");
	if (Command.Parameters.Contains(TEXT("slot")))
	{
		SlotName = Command.Parameters[TEXT("slot")];
	}

	// KScriptSubsystemを取得
	UWorld* World = Engine->GetWorld();
	if (!World)
	{
		UE_LOG(LogKScript, Error, TEXT("[save] ワールドが取得できません"));
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogKScript, Error, TEXT("[save] ゲームインスタンスが取得できません"));
		return;
	}

	UKScriptSubsystem* Subsystem = GameInstance->GetSubsystem<UKScriptSubsystem>();
	if (!Subsystem)
	{
		UE_LOG(LogKScript, Error, TEXT("[save] KScriptサブシステムが取得できません"));
		return;
	}

	// セーブを実行
	if (Subsystem->SaveGame(SlotName))
	{
		UE_LOG(LogKScript, Log, TEXT("[save] ゲームをセーブしました: スロット=%s"), *SlotName);
	}
	else
	{
		UE_LOG(LogKScript, Error, TEXT("[save] セーブに失敗しました: スロット=%s"), *SlotName);
	}
}
