// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/Image.h"
#include "KScriptTransitionManager.generated.h"

/**
 * トランジションの種類
 */
UENUM(BlueprintType)
enum class EKScriptTransitionType : uint8
{
	None,           // トランジションなし（即座に切り替え）
	Fade,           // フェード
	CrossFade,      // クロスフェード
	SlideLeft,      // 左へスライド
	SlideRight,     // 右へスライド
	SlideUp,        // 上へスライド
	SlideDown       // 下へスライド
};

/**
 * トランジション情報
 */
USTRUCT(BlueprintType)
struct KSCRIPT_API FKScriptTransitionInfo
{
	GENERATED_BODY()

	/** トランジションの種類 */
	UPROPERTY(BlueprintReadWrite, Category = "Transition")
	EKScriptTransitionType Type = EKScriptTransitionType::Fade;

	/** トランジション時間（秒） */
	UPROPERTY(BlueprintReadWrite, Category = "Transition")
	float Duration = 0.5f;

	/** ターゲットウィジェット */
	UPROPERTY()
	TObjectPtr<UImage> TargetWidget = nullptr;

	/** 開始時の不透明度 */
	UPROPERTY()
	float StartOpacity = 0.0f;

	/** 終了時の不透明度 */
	UPROPERTY()
	float EndOpacity = 1.0f;

	/** 開始時の位置 */
	UPROPERTY()
	FVector2D StartPosition = FVector2D::ZeroVector;

	/** 終了時の位置 */
	UPROPERTY()
	FVector2D EndPosition = FVector2D::ZeroVector;

	/** 経過時間 */
	UPROPERTY()
	float ElapsedTime = 0.0f;

	/** トランジション完了フラグ */
	UPROPERTY()
	bool bCompleted = false;
};

/**
 * トランジション管理システム
 * 画像の表示切り替え時にフェードやスライドなどのエフェクトを適用
 */
UCLASS()
class KSCRIPT_API UKScriptTransitionManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatTracker& GetStatId() const override;
	// End of USubsystem interface

	/**
	 * トランジションを開始する
	 * @param TargetWidget ターゲットウィジェット
	 * @param TransitionType トランジションの種類
	 * @param Duration トランジション時間（秒）
	 * @param StartOpacity 開始時の不透明度
	 * @param EndOpacity 終了時の不透明度
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Transition")
	void StartTransition(UImage* TargetWidget, EKScriptTransitionType TransitionType, float Duration, float StartOpacity = 0.0f, float EndOpacity = 1.0f);

	/**
	 * トランジションが実行中かどうか
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|Transition")
	bool IsTransitioning() const { return ActiveTransitions.Num() > 0; }

	/**
	 * すべてのトランジションをキャンセル
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Transition")
	void CancelAllTransitions();

protected:
	/**
	 * トランジションを更新
	 */
	void UpdateTransition(FKScriptTransitionInfo& Transition, float DeltaTime);

	/**
	 * フェードトランジションを適用
	 */
	void ApplyFadeTransition(FKScriptTransitionInfo& Transition);

	/**
	 * スライドトランジションを適用
	 */
	void ApplySlideTransition(FKScriptTransitionInfo& Transition);

protected:
	/** アクティブなトランジション情報 */
	UPROPERTY()
	TArray<FKScriptTransitionInfo> ActiveTransitions;

	/** Tick有効フラグ */
	bool bTickEnabled = false;
};
