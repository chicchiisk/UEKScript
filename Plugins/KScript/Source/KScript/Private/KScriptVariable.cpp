// Copyright Koromosoft. All Rights Reserved.

#include "KScriptVariable.h"

UKScriptVariable::UKScriptVariable()
{
}

void UKScriptVariable::SetInt(const FString& VarName, int32 Value)
{
	Variables.Emplace(VarName, FKScriptVariableValue(Value));
	UE_LOG(LogTemp, Verbose, TEXT("KScriptVariable: Set %s = %d"), *VarName, Value);
}

void UKScriptVariable::SetFloat(const FString& VarName, float Value)
{
	Variables.Emplace(VarName, FKScriptVariableValue(Value));
	UE_LOG(LogTemp, Verbose, TEXT("KScriptVariable: Set %s = %f"), *VarName, Value);
}

void UKScriptVariable::SetString(const FString& VarName, const FString& Value)
{
	Variables.Emplace(VarName, FKScriptVariableValue(Value));
	UE_LOG(LogTemp, Verbose, TEXT("KScriptVariable: Set %s = %s"), *VarName, *Value);
}

void UKScriptVariable::SetBool(const FString& VarName, bool Value)
{
	Variables.Emplace(VarName, FKScriptVariableValue(Value));
	UE_LOG(LogTemp, Verbose, TEXT("KScriptVariable: Set %s = %s"), *VarName, Value ? TEXT("true") : TEXT("false"));
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
	UE_LOG(LogTemp, Verbose, TEXT("KScriptVariable: Removed %s"), *VarName);
}

void UKScriptVariable::ClearAll()
{
	Variables.Empty();
	UE_LOG(LogTemp, Log, TEXT("KScriptVariable: Cleared all variables"));
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
	Variables.Emplace(VarName, Value);
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
	if (TrimmedExpr.Contains(TEXT(".")))
	{
		FKScriptVariableValue VarValue = GetVariableByPath(TrimmedExpr);
		const FKScriptVariableValue* FoundValue = Variables.Find(TrimmedExpr);
		if (FoundValue)
		{
			OutResult = VarValue;
			return true;
		}
	}

	// 簡易的な算術演算（+, -, *, /）
	for (const TCHAR* Op : { TEXT("+"), TEXT("-"), TEXT("*"), TEXT("/") })
	{
		int32 OpPos = TrimmedExpr.Find(Op);
		if (OpPos != INDEX_NONE)
		{
			FString Left = TrimmedExpr.Left(OpPos).TrimStartAndEnd();
			FString Right = TrimmedExpr.Mid(OpPos + 1).TrimStartAndEnd();

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
				else if (FCString::Strcmp(Op, TEXT("*")) == 0)
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

	// 簡易的な比較演算（==, !=, >=, <=, >, <）
	// 注: >=, <= を >, < より先にチェックする必要がある
	for (const TCHAR* Op : { TEXT("=="), TEXT("!="), TEXT(">="), TEXT("<="), TEXT(">"), TEXT("<") })
	{
		int32 OpPos = TrimmedExpr.Find(Op);
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

	UE_LOG(LogTemp, Warning, TEXT("KScriptVariable: Failed to evaluate expression: %s"), *Expression);
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
