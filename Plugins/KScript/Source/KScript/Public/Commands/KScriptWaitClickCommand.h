// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptWaitClickCommand.generated.h"

/**
 * クリック待ちコマンド [l]
 */
UCLASS()
class KSCRIPT_API UKScriptWaitClickCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(class UKScriptEngine* Engine, const FKScriptCommand* Command, class UKScriptVariable* VariableManager) override;
	virtual EKScriptCommandType GetCommandType() const override { return EKScriptCommandType::WaitClick; }
};
