// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptAudioManager.h"
#include "KScript.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UKScriptAudioManager::UKScriptAudioManager()
{
}

void UKScriptAudioManager::Initialize(UWorld* InWorld)
{
	World = InWorld;
	UE_LOG(LogKScript, Log, TEXT("KScriptAudioManagerを初期化しました"));
}

USoundBase* UKScriptAudioManager::LoadSound(const FString& SoundPath)
{
	if (SoundPath.IsEmpty())
	{
		UE_LOG(LogKScript, Warning, TEXT("サウンドパスが空です"));
		return nullptr;
	}

	// アセットパスからサウンドをロード
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *SoundPath);

	if (!Sound)
	{
		UE_LOG(LogKScript, Warning, TEXT("サウンドのロードに失敗しました: %s"), *SoundPath);
		return nullptr;
	}

	UE_LOG(LogKScript, Log, TEXT("サウンドをロードしました: %s"), *SoundPath);
	return Sound;
}

bool UKScriptAudioManager::PlayBGM(const FString& SoundPath, bool bLoop, float Volume)
{
	if (!World.IsValid())
	{
		UE_LOG(LogKScript, Error, TEXT("Worldが無効です"));
		return false;
	}

	USoundBase* Sound = LoadSound(SoundPath);
	if (!Sound)
	{
		return false;
	}

	// 既存のBGMを停止
	if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->Stop();
	}

	// 新しいBGMを再生
	BGMAudioComponent = UGameplayStatics::SpawnSound2D(World.Get(), Sound, Volume);
	if (!BGMAudioComponent)
	{
		UE_LOG(LogKScript, Error, TEXT("BGMオーディオコンポーネントの作成に失敗しました"));
		return false;
	}

	BGMAudioComponent->bIsUISound = true;
	BGMAudioComponent->bAutoDestroy = false;
	// BGMAudioComponent->bLooping = true; // ループ設定を適用

	CurrentBGM = Sound;

	UE_LOG(LogKScript, Log, TEXT("BGMを再生しました: %s (Loop: %d, Volume: %f)"), *SoundPath, bLoop, Volume);
	return true;
}

void UKScriptAudioManager::StopBGM(float FadeOutDuration)
{
	if (!BGMAudioComponent || !BGMAudioComponent->IsPlaying())
	{
		return;
	}

	if (FadeOutDuration > 0.0f)
	{
		BGMAudioComponent->FadeOut(FadeOutDuration, 0.0f);
	}
	else
	{
		BGMAudioComponent->Stop();
	}

	CurrentBGM = nullptr;
	UE_LOG(LogKScript, Log, TEXT("BGMを停止しました (FadeOut: %f秒)"), FadeOutDuration);
}

bool UKScriptAudioManager::PlaySE(const FString& SoundPath, float Volume)
{
	if (!World.IsValid())
	{
		UE_LOG(LogKScript, Error, TEXT("Worldが無効です"));
		return false;
	}

	USoundBase* Sound = LoadSound(SoundPath);
	if (!Sound)
	{
		return false;
	}

	// SEを再生（ワンショット、自動破棄）
	UGameplayStatics::PlaySound2D(World.Get(), Sound, Volume);

	UE_LOG(LogKScript, Log, TEXT("SEを再生しました: %s (Volume: %f)"), *SoundPath, Volume);
	return true;
}

void UKScriptAudioManager::SetBGMVolume(float Volume)
{
	Volume = FMath::Clamp(Volume, 0.0f, 1.0f);

	if (BGMAudioComponent)
	{
		BGMAudioComponent->SetVolumeMultiplier(Volume);
		UE_LOG(LogKScript, Log, TEXT("BGMボリュームを %f に設定しました"), Volume);
	}
}

bool UKScriptAudioManager::IsBGMPlaying() const
{
	return BGMAudioComponent && BGMAudioComponent->IsPlaying();
}
