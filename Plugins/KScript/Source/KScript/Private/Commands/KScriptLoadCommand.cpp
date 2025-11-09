// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commands/KScriptLoadCommand.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptSubsystem.h"
#include "Engine/GameInstance.h"

void UKScriptLoadCommand::Execute(UKScriptEngine* Engine, const FKScriptCommand* Command, UKScriptVariable* VariableManager)
{
	if (!Engine)
	{
		return;
	}

	// スロット名を取得（デフォルトは"QuickSave"）
	FString SlotName = TEXT("QuickSave");
	if (Command->Parameters.Contains(TEXT("slot")))
	{
		SlotName = Command->Parameters[TEXT("slot")];
	}

	// KScriptSubsystemを取得
	UWorld* World = Engine->GetWorld();
	if (!World)
	{
		UE_LOG(LogKScript, Error, TEXT("[load] ワールドが取得できません"));
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogKScript, Error, TEXT("[load] ゲームインスタンスが取得できません"));
		return;
	}

	UKScriptSubsystem* Subsystem = GameInstance->GetSubsystem<UKScriptSubsystem>();
	if (!Subsystem)
	{
		UE_LOG(LogKScript, Error, TEXT("[load] KScriptサブシステムが取得できません"));
		return;
	}

	// ロードを実行
	if (Subsystem->LoadGame(SlotName))
	{
		UE_LOG(LogKScript, Log, TEXT("[load] ゲームをロードしました: スロット=%s"), *SlotName);
	}
	else
	{
		UE_LOG(LogKScript, Error, TEXT("[load] ロードに失敗しました: スロット=%s"), *SlotName);
	}
}
