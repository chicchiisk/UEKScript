// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptTransitionManager.h"
#include "KScript.h"
#include "Components/CanvasPanelSlot.h"

DECLARE_STATS_GROUP(TEXT("KScript"), STATGROUP_KScript, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("TransitionManager Tick"), STAT_TransitionManagerTick, STATGROUP_KScript);

void UKScriptTransitionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogKScript, Log, TEXT("KScriptTransitionManagerを初期化しました"));
}

void UKScriptTransitionManager::Deinitialize()
{
	CancelAllTransitions();
	UE_LOG(LogKScript, Log, TEXT("KScriptTransitionManagerを終了しました"));
	Super::Deinitialize();
}

void UKScriptTransitionManager::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_TransitionManagerTick);

	if (ActiveTransitions.Num() == 0)
	{
		return;
	}

	// トランジションを更新
	for (int32 i = ActiveTransitions.Num() - 1; i >= 0; --i)
	{
		FKScriptTransitionInfo& Transition = ActiveTransitions[i];

		if (!Transition.TargetWidget || !Transition.TargetWidget->IsValidLowLevel())
		{
			// ウィジェットが無効になった場合は削除
			ActiveTransitions.RemoveAt(i);
			continue;
		}

		UpdateTransition(Transition, DeltaTime);

		// 完了したトランジションを削除
		if (Transition.bCompleted)
		{
			UE_LOG(LogKScript, Log, TEXT("トランジション完了: Type=%d"), (int32)Transition.Type);
			ActiveTransitions.RemoveAt(i);
		}
	}
}

TStatId UKScriptTransitionManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UKScriptTransitionManager, STATGROUP_Tickables);
}

void UKScriptTransitionManager::StartTransition(UImage* TargetWidget, EKScriptTransitionType TransitionType, float Duration, float StartOpacity, float EndOpacity)
{
	if (!TargetWidget)
	{
		UE_LOG(LogKScript, Warning, TEXT("トランジション開始失敗: ターゲットウィジェットがnullです"));
		return;
	}

	if (TransitionType == EKScriptTransitionType::None)
	{
		// トランジションなしの場合は即座に適用
		TargetWidget->SetOpacity(EndOpacity);
		return;
	}

	// 新しいトランジション情報を作成
	FKScriptTransitionInfo NewTransition;
	NewTransition.Type = TransitionType;
	NewTransition.Duration = FMath::Max(Duration, 0.01f); // 最小値を設定
	NewTransition.TargetWidget = TargetWidget;
	NewTransition.StartOpacity = StartOpacity;
	NewTransition.EndOpacity = EndOpacity;
	NewTransition.ElapsedTime = 0.0f;
	NewTransition.bCompleted = false;

	// スライド系トランジションの場合は開始・終了位置を設定
	if (TransitionType == EKScriptTransitionType::SlideLeft ||
		TransitionType == EKScriptTransitionType::SlideRight ||
		TransitionType == EKScriptTransitionType::SlideUp ||
		TransitionType == EKScriptTransitionType::SlideDown)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
		if (CanvasSlot)
		{
			NewTransition.EndPosition = CanvasSlot->GetPosition();

			// 開始位置を計算
			FVector2D Offset = FVector2D::ZeroVector;
			switch (TransitionType)
			{
			case EKScriptTransitionType::SlideLeft:
				Offset = FVector2D(500.0f, 0.0f);
				break;
			case EKScriptTransitionType::SlideRight:
				Offset = FVector2D(-500.0f, 0.0f);
				break;
			case EKScriptTransitionType::SlideUp:
				Offset = FVector2D(0.0f, 500.0f);
				break;
			case EKScriptTransitionType::SlideDown:
				Offset = FVector2D(0.0f, -500.0f);
				break;
			default:
				break;
			}
			NewTransition.StartPosition = NewTransition.EndPosition + Offset;

			// 初期位置を設定
			CanvasSlot->SetPosition(NewTransition.StartPosition);
		}
	}

	// 初期状態を適用
	TargetWidget->SetOpacity(StartOpacity);

	ActiveTransitions.Add(NewTransition);
	UE_LOG(LogKScript, Log, TEXT("トランジション開始: Type=%d, Duration=%.2f秒"), (int32)TransitionType, Duration);
}

void UKScriptTransitionManager::CancelAllTransitions()
{
	ActiveTransitions.Empty();
	UE_LOG(LogKScript, Log, TEXT("すべてのトランジションをキャンセルしました"));
}

void UKScriptTransitionManager::UpdateTransition(FKScriptTransitionInfo& Transition, float DeltaTime)
{
	Transition.ElapsedTime += DeltaTime;

	// 進捗率を計算（0.0 〜 1.0）
	float Progress = FMath::Clamp(Transition.ElapsedTime / Transition.Duration, 0.0f, 1.0f);

	// イージング関数を適用（EaseInOut）
	float EasedProgress = FMath::InterpEaseInOut(0.0f, 1.0f, Progress, 2.0f);

	// トランジションタイプに応じて処理
	switch (Transition.Type)
	{
	case EKScriptTransitionType::Fade:
	case EKScriptTransitionType::CrossFade:
		{
			float CurrentOpacity = FMath::Lerp(Transition.StartOpacity, Transition.EndOpacity, EasedProgress);
			Transition.TargetWidget->SetOpacity(CurrentOpacity);
		}
		break;

	case EKScriptTransitionType::SlideLeft:
	case EKScriptTransitionType::SlideRight:
	case EKScriptTransitionType::SlideUp:
	case EKScriptTransitionType::SlideDown:
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Transition.TargetWidget->Slot);
			if (CanvasSlot)
			{
				FVector2D CurrentPosition = FMath::Lerp(Transition.StartPosition, Transition.EndPosition, EasedProgress);
				CanvasSlot->SetPosition(CurrentPosition);
			}

			// スライド中も不透明度を変化させる
			float CurrentOpacity = FMath::Lerp(Transition.StartOpacity, Transition.EndOpacity, EasedProgress);
			Transition.TargetWidget->SetOpacity(CurrentOpacity);
		}
		break;

	default:
		break;
	}

	// トランジション完了判定
	if (Progress >= 1.0f)
	{
		// 最終状態を適用
		Transition.TargetWidget->SetOpacity(Transition.EndOpacity);

		if (Transition.Type == EKScriptTransitionType::SlideLeft ||
			Transition.Type == EKScriptTransitionType::SlideRight ||
			Transition.Type == EKScriptTransitionType::SlideUp ||
			Transition.Type == EKScriptTransitionType::SlideDown)
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Transition.TargetWidget->Slot);
			if (CanvasSlot)
			{
				CanvasSlot->SetPosition(Transition.EndPosition);
			}
		}

		Transition.bCompleted = true;
	}
}

void UKScriptTransitionManager::ApplyFadeTransition(FKScriptTransitionInfo& Transition)
{
	// UpdateTransitionで処理済み
}

void UKScriptTransitionManager::ApplySlideTransition(FKScriptTransitionInfo& Transition)
{
	// UpdateTransitionで処理済み
}
