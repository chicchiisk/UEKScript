// Copyright Koromosoft. All Rights Reserved.

#include "KScriptSubsystem.h"
#include "KScriptEngine.h"
#include "KScriptParser.h"
#include "KScriptVariable.h"

UKScriptSubsystem::UKScriptSubsystem()
{
}

void UKScriptSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// KScriptコンポーネントを初期化
	ScriptEngine = NewObject<UKScriptEngine>(this);
	ScriptParser = NewObject<UKScriptParser>(this);
	VariableManager = NewObject<UKScriptVariable>(this);

	// エンジンにパーサーと変数管理システムを設定
	ScriptEngine->SetParser(ScriptParser);
	ScriptEngine->SetVariableManager(VariableManager);

	UE_LOG(LogTemp, Log, TEXT("KScriptSubsystem: Initialized"));
}

void UKScriptSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("KScriptSubsystem: Deinitialized"));

	Super::Deinitialize();
}

bool UKScriptSubsystem::LoadAndStartScript(const FString& FilePath)
{
	if (!ScriptEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("KScriptSubsystem: ScriptEngine is not initialized"));
		return false;
	}

	if (ScriptEngine->LoadScriptFromFile(FilePath))
	{
		ScriptEngine->Start();
		UE_LOG(LogTemp, Log, TEXT("KScriptSubsystem: Started script from file: %s"), *FilePath);
		return true;
	}

	return false;
}

bool UKScriptSubsystem::LoadAndStartScriptFromText(const FString& ScriptText)
{
	if (!ScriptEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("KScriptSubsystem: ScriptEngine is not initialized"));
		return false;
	}

	if (ScriptEngine->LoadScript(ScriptText))
	{
		ScriptEngine->Start();
		UE_LOG(LogTemp, Log, TEXT("KScriptSubsystem: Started script from text"));
		return true;
	}

	return false;
}

void UKScriptSubsystem::PauseScript()
{
	if (ScriptEngine)
	{
		ScriptEngine->Pause();
	}
}

void UKScriptSubsystem::ResumeScript()
{
	if (ScriptEngine)
	{
		ScriptEngine->Resume();
	}
}

void UKScriptSubsystem::StopScript()
{
	if (ScriptEngine)
	{
		ScriptEngine->Stop();
	}
}

void UKScriptSubsystem::HandleInput()
{
	if (ScriptEngine)
	{
		ScriptEngine->OnInput();
	}
}

bool UKScriptSubsystem::StepScript()
{
	if (ScriptEngine)
	{
		return ScriptEngine->Step();
	}

	return false;
}
