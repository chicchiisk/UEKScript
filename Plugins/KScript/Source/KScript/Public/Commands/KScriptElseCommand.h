// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptElseCommand.generated.h"

/**
 * Elseコマンド [else]
 */
UCLASS()
class KSCRIPT_API UKScriptElseCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Else; }
};
