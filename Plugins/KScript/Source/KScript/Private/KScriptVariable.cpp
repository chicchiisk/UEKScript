// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptVariable.h"
#include "KScript.h"

UKScriptVariable::UKScriptVariable()
{
}

void UKScriptVariable::SetInt(const FString& VarName, int32 Value)
{
	Variables.FindOrAdd(VarName) = FKScriptVariableValue(Value);
	UE_LOG(LogKScript, Verbose, TEXT("変数を設定: %s = %d"), *VarName, Value);
}

void UKScriptVariable::SetFloat(const FString& VarName, float Value)
{
	Variables.FindOrAdd(VarName) = FKScriptVariableValue(Value);
	UE_LOG(LogKScript, Verbose, TEXT("変数を設定: %s = %f"), *VarName, Value);
}

void UKScriptVariable::SetString(const FString& VarName, const FString& Value)
{
	Variables.FindOrAdd(VarName) = FKScriptVariableValue(Value);
	UE_LOG(LogKScript, Verbose, TEXT("変数を設定: %s = %s"), *VarName, *Value);
}

void UKScriptVariable::SetBool(const FString& VarName, bool Value)
{
	Variables.FindOrAdd(VarName) = FKScriptVariableValue(Value);
	UE_LOG(LogKScript, Verbose, TEXT("変数を設定: %s = %s"), *VarName, Value ? TEXT("true") : TEXT("false"));
}

int32 UKScriptVariable::GetInt(const FString& VarName, int32 DefaultValue) const
{
	const FKScriptVariableValue* Value = Variables.Find(VarName);
	return Value ? Value->AsInt() : DefaultValue;
}

float UKScriptVariable::GetFloat(const FString& VarName, float DefaultValue) const
{
	const FKScriptVariableValue* Value = Variables.Find(VarName);
	return Value ? Value->AsFloat() : DefaultValue;
}

FString UKScriptVariable::GetString(const FString& VarName, const FString& DefaultValue) const
{
	const FKScriptVariableValue* Value = Variables.Find(VarName);
	return Value ? Value->AsString() : DefaultValue;
}

bool UKScriptVariable::GetBool(const FString& VarName, bool DefaultValue) const
{
	const FKScriptVariableValue* Value = Variables.Find(VarName);
	return Value ? Value->AsBool() : DefaultValue;
}

bool UKScriptVariable::HasVariable(const FString& VarName) const
{
	return Variables.Contains(VarName);
}

void UKScriptVariable::RemoveVariable(const FString& VarName)
{
	Variables.Remove(VarName);
	UE_LOG(LogKScript, Verbose, TEXT("変数を削除: %s"), *VarName);
}

void UKScriptVariable::ClearAll()
{
	Variables.Empty();
	UE_LOG(LogKScript, Log, TEXT("すべての変数をクリアしました"));
}

bool UKScriptVariable::EvaluateExpression(const FString& Expression, FKScriptVariableValue& OutResult)
{
	// 簡易的な式評価
	return EvaluateSimpleExpression(Expression, OutResult);
}

FString UKScriptVariable::EmbedVariables(const FString& Text)
{
	// [emb exp="変数名"]の形式を変数値で置き換える
	FString Result = Text;

	// TODO: より高度な埋め込み処理を実装
	// 現在は簡易版として、変数名を直接置き換える

	return Result;
}

const FKScriptVariableValue* UKScriptVariable::GetVariableValue(const FString& VarName) const
{
	return Variables.Find(VarName);
}

void UKScriptVariable::SetVariableValue(const FString& VarName, const FKScriptVariableValue& Value)
{
	Variables.FindOrAdd(VarName) = Value;
}

bool UKScriptVariable::EvaluateSimpleExpression(const FString& Expression, FKScriptVariableValue& OutResult)
{
	FString TrimmedExpr = Expression.TrimStartAndEnd();

	// 数値リテラル
	if (TrimmedExpr.IsNumeric())
	{
		if (TrimmedExpr.Contains(TEXT(".")))
		{
			OutResult = FKScriptVariableValue(FCString::Atof(*TrimmedExpr));
		}
		else
		{
			OutResult = FKScriptVariableValue(FCString::Atoi(*TrimmedExpr));
		}
		return true;
	}

	// 文字列リテラル
	if (TrimmedExpr.StartsWith(TEXT("\"")) && TrimmedExpr.EndsWith(TEXT("\"")))
	{
		FString StringValue = TrimmedExpr.Mid(1, TrimmedExpr.Len() - 2);
		OutResult = FKScriptVariableValue(StringValue);
		return true;
	}

	// 真偽値リテラル
	if (TrimmedExpr.Equals(TEXT("true"), ESearchCase::IgnoreCase))
	{
		OutResult = FKScriptVariableValue(true);
		return true;
	}
	if (TrimmedExpr.Equals(TEXT("false"), ESearchCase::IgnoreCase))
	{
		OutResult = FKScriptVariableValue(false);
		return true;
	}

	// 変数参照
	if (HasVariable(TrimmedExpr))
	{
		const FKScriptVariableValue* Value = GetVariableValue(TrimmedExpr);
		if (Value)
		{
			OutResult = *Value;
			return true;
		}
	}

	// ドット記法の変数参照（例: f.hp）
	// 変数が存在するかチェックしてから値を取得
	if (Variables.Contains(TrimmedExpr))
	{
		OutResult = GetVariableByPath(TrimmedExpr);
		return true;
	}

	// 算術演算子を検索（文字列リテラル内を除く）
	auto FindOperatorOutsideQuotes = [](const FString& Expr, const TCHAR* Op) -> int32
	{
		bool InQuotes = false;
		for (int32 i = 0; i < Expr.Len(); ++i)
		{
			if (Expr[i] == TEXT('"'))
			{
				InQuotes = !InQuotes;
			}
			else if (!InQuotes && FCString::Strncmp(&Expr[i], Op, FCString::Strlen(Op)) == 0)
			{
				return i;
			}
		}
		return INDEX_NONE;
	};

	// 簡易的な算術演算（優先順位を考慮: 低い順に +, - → *, /）
	// 優先順位の低い演算子（+, -）を先に検索
	for (const TCHAR* Op : { TEXT("+"), TEXT("-") })
	{
		int32 OpPos = FindOperatorOutsideQuotes(TrimmedExpr, Op);
		if (OpPos != INDEX_NONE)
		{
			FString Left = TrimmedExpr.Left(OpPos).TrimStartAndEnd();
			FString Right = TrimmedExpr.Mid(OpPos + FCString::Strlen(Op)).TrimStartAndEnd();

			FKScriptVariableValue LeftValue, RightValue;
			if (EvaluateSimpleExpression(Left, LeftValue) && EvaluateSimpleExpression(Right, RightValue))
			{
				float LeftNum = LeftValue.AsFloat();
				float RightNum = RightValue.AsFloat();
				float ResultNum = 0.0f;

				if (FCString::Strcmp(Op, TEXT("+")) == 0)
					ResultNum = LeftNum + RightNum;
				else if (FCString::Strcmp(Op, TEXT("-")) == 0)
					ResultNum = LeftNum - RightNum;

				// 結果が整数の場合は整数型で返す
				if (FMath::IsNearlyEqual(ResultNum, FMath::RoundToFloat(ResultNum)))
				{
					OutResult = FKScriptVariableValue(static_cast<int32>(ResultNum));
				}
				else
				{
					OutResult = FKScriptVariableValue(ResultNum);
				}
				return true;
			}
		}
	}

	// 優先順位の高い演算子（*, /）
	for (const TCHAR* Op : { TEXT("*"), TEXT("/") })
	{
		int32 OpPos = FindOperatorOutsideQuotes(TrimmedExpr, Op);
		if (OpPos != INDEX_NONE)
		{
			FString Left = TrimmedExpr.Left(OpPos).TrimStartAndEnd();
			FString Right = TrimmedExpr.Mid(OpPos + FCString::Strlen(Op)).TrimStartAndEnd();

			FKScriptVariableValue LeftValue, RightValue;
			if (EvaluateSimpleExpression(Left, LeftValue) && EvaluateSimpleExpression(Right, RightValue))
			{
				float LeftNum = LeftValue.AsFloat();
				float RightNum = RightValue.AsFloat();
				float ResultNum = 0.0f;

				if (FCString::Strcmp(Op, TEXT("*")) == 0)
					ResultNum = LeftNum * RightNum;
				else if (FCString::Strcmp(Op, TEXT("/")) == 0)
					ResultNum = RightNum != 0.0f ? LeftNum / RightNum : 0.0f;

				// 結果が整数の場合は整数型で返す
				if (FMath::IsNearlyEqual(ResultNum, FMath::RoundToFloat(ResultNum)))
				{
					OutResult = FKScriptVariableValue(static_cast<int32>(ResultNum));
				}
				else
				{
					OutResult = FKScriptVariableValue(ResultNum);
				}
				return true;
			}
		}
	}

	// 簡易的な比較演算（==, !=, >, <, >=, <=）
	// 文字列リテラル内の演算子を除外
	for (const TCHAR* Op : { TEXT("=="), TEXT("!="), TEXT(">="), TEXT("<="), TEXT(">"), TEXT("<") })
	{
		int32 OpPos = FindOperatorOutsideQuotes(TrimmedExpr, Op);
		if (OpPos != INDEX_NONE)
		{
			FString Left = TrimmedExpr.Left(OpPos).TrimStartAndEnd();
			int32 OpLen = FCString::Strlen(Op);
			FString Right = TrimmedExpr.Mid(OpPos + OpLen).TrimStartAndEnd();

			FKScriptVariableValue LeftValue, RightValue;
			if (EvaluateSimpleExpression(Left, LeftValue) && EvaluateSimpleExpression(Right, RightValue))
			{
				bool Result = false;

				if (FCString::Strcmp(Op, TEXT("==")) == 0)
					Result = LeftValue.AsFloat() == RightValue.AsFloat();
				else if (FCString::Strcmp(Op, TEXT("!=")) == 0)
					Result = LeftValue.AsFloat() != RightValue.AsFloat();
				else if (FCString::Strcmp(Op, TEXT(">")) == 0)
					Result = LeftValue.AsFloat() > RightValue.AsFloat();
				else if (FCString::Strcmp(Op, TEXT("<")) == 0)
					Result = LeftValue.AsFloat() < RightValue.AsFloat();
				else if (FCString::Strcmp(Op, TEXT(">=")) == 0)
					Result = LeftValue.AsFloat() >= RightValue.AsFloat();
				else if (FCString::Strcmp(Op, TEXT("<=")) == 0)
					Result = LeftValue.AsFloat() <= RightValue.AsFloat();

				OutResult = FKScriptVariableValue(Result);
				return true;
			}
		}
	}

	UE_LOG(LogKScript, Warning, TEXT("式の評価に失敗しました: %s"), *Expression);
	return false;
}

FKScriptVariableValue UKScriptVariable::GetVariableByPath(const FString& VarPath) const
{
	// ドット記法に対応（例: f.hp は "f.hp" という変数名として扱う）
	const FKScriptVariableValue* Value = Variables.Find(VarPath);
	if (Value)
	{
		return *Value;
	}

	// デフォルト値を返す
	return FKScriptVariableValue(0);
}
