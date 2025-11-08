// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptIfCommand.generated.h"

/**
 * 条件分岐コマンド [if]
 */
UCLASS()
class KSCRIPT_API UKScriptIfCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::If; }
};
