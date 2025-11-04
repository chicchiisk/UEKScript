// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptSubsystem.h"
#include "KScript.h"
#include "KScriptEngine.h"
#include "KScriptParser.h"
#include "KScriptVariable.h"
#include "KScriptAsset.h"
#include "KScriptUIManager.h"

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

	// UIManagerをSubsystemから取得して設定
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UKScriptUIManager* UIManager = GameInstance->GetSubsystem<UKScriptUIManager>();
		if (UIManager)
		{
			ScriptEngine->SetUIManager(UIManager);
			UE_LOG(LogKScript, Log, TEXT("KScriptEngine にUIManagerを設定しました"));
		}
	}

	UE_LOG(LogKScript, Log, TEXT("KScriptサブシステムを初期化しました"));
}

void UKScriptSubsystem::Deinitialize()
{
	UE_LOG(LogKScript, Log, TEXT("KScriptサブシステムを終了しました"));

	Super::Deinitialize();
}

bool UKScriptSubsystem::LoadAndStartScript(const FString& FilePath)
{
	if (!ScriptEngine)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトエンジンが初期化されていません"));
		return false;
	}

	if (ScriptEngine->LoadScriptFromFile(FilePath))
	{
		ScriptEngine->Start();
		UE_LOG(LogKScript, Log, TEXT("スクリプトファイルから実行を開始しました: %s"), *FilePath);
		return true;
	}

	return false;
}

bool UKScriptSubsystem::LoadAndStartScriptFromAsset(UKScriptAsset* ScriptAsset)
{
	if (!ScriptEngine)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトエンジンが初期化されていません"));
		return false;
	}

	if (!ScriptAsset)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトアセットがnullです"));
		return false;
	}

	if (ScriptEngine->LoadScriptFromAsset(ScriptAsset))
	{
		ScriptEngine->Start();
		UE_LOG(LogKScript, Log, TEXT("スクリプトアセットから実行を開始しました: %s"), *ScriptAsset->GetName());
		return true;
	}

	return false;
}

bool UKScriptSubsystem::LoadAndStartScriptFromText(const FString& ScriptText)
{
	if (!ScriptEngine)
	{
		UE_LOG(LogKScript, Error, TEXT("スクリプトエンジンが初期化されていません"));
		return false;
	}

	if (ScriptEngine->LoadScript(ScriptText))
	{
		ScriptEngine->Start();
		UE_LOG(LogKScript, Log, TEXT("テキストからスクリプト実行を開始しました"));
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
