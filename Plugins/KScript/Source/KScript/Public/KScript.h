// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

// KScriptログカテゴリの定義
DECLARE_LOG_CATEGORY_EXTERN(LogKScript, Log, All);

class FKScriptModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
