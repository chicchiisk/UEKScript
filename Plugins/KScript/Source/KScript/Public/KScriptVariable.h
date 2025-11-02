// Copyright Koromosoft. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KScriptVariable.generated.h"

/**
 * 変数の値の型
 */
UENUM(BlueprintType)
enum class EKScriptVariableType : uint8
{
	Integer,    // 整数
	Float,      // 浮動小数点数
	String,     // 文字列
	Boolean     // 真偽値
};

/**
 * 変数の値
 */
USTRUCT(BlueprintType)
struct KSCRIPT_API FKScriptVariableValue
{
	GENERATED_BODY()

	UPROPERTY()
	EKScriptVariableType Type = EKScriptVariableType::Integer;

	UPROPERTY()
	int32 IntValue = 0;

	UPROPERTY()
	float FloatValue = 0.0f;

	UPROPERTY()
	FString StringValue;

	UPROPERTY()
	bool BoolValue = false;

	FKScriptVariableValue() = default;

	// コンストラクタ
	explicit FKScriptVariableValue(int32 Value)
		: Type(EKScriptVariableType::Integer), IntValue(Value) {}

	explicit FKScriptVariableValue(float Value)
		: Type(EKScriptVariableType::Float), FloatValue(Value) {}

	explicit FKScriptVariableValue(const FString& Value)
		: Type(EKScriptVariableType::String), StringValue(Value) {}

	explicit FKScriptVariableValue(bool Value)
		: Type(EKScriptVariableType::Boolean), BoolValue(Value) {}

	// 整数として取得
	int32 AsInt() const
	{
		switch (Type)
		{
		case EKScriptVariableType::Integer: return IntValue;
		case EKScriptVariableType::Float: return static_cast<int32>(FloatValue);
		case EKScriptVariableType::String: return FCString::Atoi(*StringValue);
		case EKScriptVariableType::Boolean: return BoolValue ? 1 : 0;
		default: return 0;
		}
	}

	// 浮動小数点数として取得
	float AsFloat() const
	{
		switch (Type)
		{
		case EKScriptVariableType::Integer: return static_cast<float>(IntValue);
		case EKScriptVariableType::Float: return FloatValue;
		case EKScriptVariableType::String: return FCString::Atof(*StringValue);
		case EKScriptVariableType::Boolean: return BoolValue ? 1.0f : 0.0f;
		default: return 0.0f;
		}
	}

	// 文字列として取得
	FString AsString() const
	{
		switch (Type)
		{
		case EKScriptVariableType::Integer: return FString::FromInt(IntValue);
		case EKScriptVariableType::Float: return FString::SanitizeFloat(FloatValue);
		case EKScriptVariableType::String: return StringValue;
		case EKScriptVariableType::Boolean: return BoolValue ? TEXT("true") : TEXT("false");
		default: return TEXT("");
		}
	}

	// 真偽値として取得
	bool AsBool() const
	{
		switch (Type)
		{
		case EKScriptVariableType::Integer: return IntValue != 0;
		case EKScriptVariableType::Float: return FloatValue != 0.0f;
		case EKScriptVariableType::String: return !StringValue.IsEmpty();
		case EKScriptVariableType::Boolean: return BoolValue;
		default: return false;
		}
	}
};

/**
 * KScript変数管理システム
 */
UCLASS(BlueprintType)
class KSCRIPT_API UKScriptVariable : public UObject
{
	GENERATED_BODY()

public:
	UKScriptVariable();

	/**
	 * 変数を設定（整数）
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Variable")
	void SetInt(const FString& VarName, int32 Value);

	/**
	 * 変数を設定（浮動小数点数）
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Variable")
	void SetFloat(const FString& VarName, float Value);

	/**
	 * 変数を設定（文字列）
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Variable")
	void SetString(const FString& VarName, const FString& Value);

	/**
	 * 変数を設定（真偽値）
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Variable")
	void SetBool(const FString& VarName, bool Value);

	/**
	 * 変数を取得（整数）
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|Variable")
	int32 GetInt(const FString& VarName, int32 DefaultValue = 0) const;

	/**
	 * 変数を取得（浮動小数点数）
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|Variable")
	float GetFloat(const FString& VarName, float DefaultValue = 0.0f) const;

	/**
	 * 変数を取得（文字列）
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|Variable")
	FString GetString(const FString& VarName, const FString& DefaultValue = TEXT("")) const;

	/**
	 * 変数を取得（真偽値）
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|Variable")
	bool GetBool(const FString& VarName, bool DefaultValue = false) const;

	/**
	 * 変数が存在するかチェック
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|Variable")
	bool HasVariable(const FString& VarName) const;

	/**
	 * 変数を削除
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Variable")
	void RemoveVariable(const FString& VarName);

	/**
	 * すべての変数をクリア
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Variable")
	void ClearAll();

	/**
	 * 式を評価
	 * @param Expression 評価する式（例: "f.hp + 10", "f.name == \"太郎\""）
	 * @param OutResult 評価結果
	 * @return 評価成功時true
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Variable")
	bool EvaluateExpression(const FString& Expression, FKScriptVariableValue& OutResult);

	/**
	 * 変数値を文字列に埋め込む
	 * @param Text 埋め込み対象のテキスト（例: "HPは[emb exp="f.hp"]です"）
	 * @return 埋め込み後のテキスト
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|Variable")
	FString EmbedVariables(const FString& Text);

	/**
	 * 変数値を直接取得
	 */
	const FKScriptVariableValue* GetVariableValue(const FString& VarName) const;

	/**
	 * 変数値を直接設定
	 */
	void SetVariableValue(const FString& VarName, const FKScriptVariableValue& Value);

protected:
	/**
	 * 簡易的な式評価（将来的にはより高度なパーサーに置き換え）
	 */
	bool EvaluateSimpleExpression(const FString& Expression, FKScriptVariableValue& OutResult);

	/**
	 * 変数名から値を取得（f.hp などのドット記法に対応）
	 */
	FKScriptVariableValue GetVariableByPath(const FString& VarPath) const;

protected:
	// 変数マップ（変数名 -> 値）
	UPROPERTY()
	TMap<FString, FKScriptVariableValue> Variables;
};
