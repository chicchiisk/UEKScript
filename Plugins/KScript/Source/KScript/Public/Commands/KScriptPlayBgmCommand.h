// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptPlayBgmCommand.generated.h"

/**
 * BGM再生コマンド [playbgm]
 */
UCLASS()
class KSCRIPT_API UKScriptPlayBgmCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::PlayBgm; }
};
