// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptEvalCommand.generated.h"

/**
 * 式評価コマンド [eval]
 */
UCLASS()
class KSCRIPT_API UKScriptEvalCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(UKScriptEngine* Engine, const FKScriptCommand& Command, UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::Eval; }
};
