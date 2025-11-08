// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptJumpCommand.generated.h"

/**
 * ジャンプコマンド [jump]
 */
UCLASS()
class KSCRIPT_API UKScriptJumpCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Jump; }
};
