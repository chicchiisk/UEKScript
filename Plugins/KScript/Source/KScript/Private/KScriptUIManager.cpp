// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptUIManager.h"
#include "KScript.h"
#include "KScriptImageManager.h"
#include "KScriptAudioManager.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

UKScriptUIManager::UKScriptUIManager()
{
}

void UKScriptUIManager::Initialize(UWorld* InWorld, UTextBlock* InMessageText, UImage* InBackgroundImage, UCanvasPanel* InCharacterContainer)
{
	MessageText = InMessageText;

	// 画像マネージャーを作成・初期化
	ImageManager = NewObject<UKScriptImageManager>(this);
	if (ImageManager)
	{
		ImageManager->Initialize(InBackgroundImage, InCharacterContainer);
	}

	// 音声マネージャーを作成・初期化
	AudioManager = NewObject<UKScriptAudioManager>(this);
	if (AudioManager)
	{
		AudioManager->Initialize(InWorld);
	}

	CurrentMessage.Empty();

	UE_LOG(LogKScript, Log, TEXT("KScriptUIManagerを初期化しました"));
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
