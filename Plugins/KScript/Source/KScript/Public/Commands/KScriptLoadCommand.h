// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commands/KScriptCommandBase.h"
#include "KScriptLoadCommand.generated.h"

/**
 * [load] コマンド - セーブデータをロードする
 *
 * 使用例:
 * [load slot="save01"]
 */
UCLASS()
class KSCRIPT_API UKScriptLoadCommand : public UKScriptCommandBase
{
	GENERATED_BODY()

public:
	virtual void Execute(const FKScriptCommand& Command, class UKScriptEngine* Engine) override;
};
