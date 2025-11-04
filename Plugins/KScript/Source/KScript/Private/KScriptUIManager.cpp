// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptUIManager.h"
#include "KScript.h"
#include "KScriptImageManager.h"
#include "KScriptAudioManager.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

void UKScriptUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CurrentMessage.Empty();

	UE_LOG(LogKScript, Log, TEXT("KScriptUIManagerサブシステムを初期化しました"));
}

void UKScriptUIManager::Deinitialize()
{
	UE_LOG(LogKScript, Log, TEXT("KScriptUIManagerサブシステムを終了しました"));

	Super::Deinitialize();
}

void UKScriptUIManager::SetupWidgets(UTextBlock* InMessageText, UImage* InBackgroundImage, UCanvasPanel* InCharacterContainer)
{
	MessageText = InMessageText;

	// ImageManagerとAudioManagerをSubsystemから取得
	UKScriptImageManager* ImageMgr = GetImageManager();
	if (ImageMgr)
	{
		ImageMgr->SetupWidgets(InBackgroundImage, InCharacterContainer);
	}

	UE_LOG(LogKScript, Log, TEXT("KScriptUIManager: ウィジェットを設定しました"));
}

UKScriptImageManager* UKScriptUIManager::GetImageManager() const
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetSubsystem<UKScriptImageManager>();
	}
	return nullptr;
}

UKScriptAudioManager* UKScriptUIManager::GetAudioManager() const
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetSubsystem<UKScriptAudioManager>();
	}
	return nullptr;
}

void UKScriptUIManager::DisplayText(const FString& Text)
{
	CurrentMessage = Text;

	if (MessageText)
	{
		MessageText->SetText(FText::FromString(Text));
	}

	UE_LOG(LogKScript, Log, TEXT("テキストを表示しました: %s"), *Text);
}

void UKScriptUIManager::AppendText(const FString& Text)
{
	CurrentMessage += Text;

	if (MessageText)
	{
		MessageText->SetText(FText::FromString(CurrentMessage));
	}

	UE_LOG(LogKScript, Log, TEXT("テキストを追加しました: %s"), *Text);
}

void UKScriptUIManager::ClearText()
{
	CurrentMessage.Empty();

	if (MessageText)
	{
		MessageText->SetText(FText::GetEmpty());
	}

	UE_LOG(LogKScript, Log, TEXT("テキストをクリアしました"));
}

void UKScriptUIManager::AddLineBreak()
{
	CurrentMessage += TEXT("\n");

	if (MessageText)
	{
		MessageText->SetText(FText::FromString(CurrentMessage));
	}

	UE_LOG(LogKScript, Log, TEXT("改行を追加しました"));
}
