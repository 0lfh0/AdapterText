// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdaptText.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "SlateGlobals.h"
#include "Framework/Text/PlainTextLayoutMarshaller.h"
#include "Widgets/Text/SlateTextBlockLayout.h"
#include "Types/ReflectionMetadata.h"
#include "Rendering/DrawElements.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
SAdaptText::SAdaptText()
{
	SetCanTick(true);
}

SAdaptText::~SAdaptText()
{

}

void SAdaptText::Construct(const FArguments& InArgs)
{
	TextStyle = *InArgs._TextStyle;
	WrapTextAt = InArgs._WrapTextAt;
	AutoWrapText = InArgs._AutoWrapText;
	WrappingPolicy = InArgs._WrappingPolicy;
	Font = InArgs._Font;
	BoundText = InArgs._Text;

	//SAdaptText
	TextAdaptType = InArgs._TextAdaptType;
	TextMarqueeDirection = InArgs._TextMarqueeDirection;
	MinFontSize = InArgs._TextMinFontSize;
	TextMarqueePolicy = InArgs._TextMarqueePolicy;
	MarqueeSpeed = InArgs._TextMarqueeSpeed;
	OnTextFontSizeChangedCallback = InArgs._OnTextFontSizeChanged;
	OnTextMarqueeEndCallback = InArgs._OnTextMarqueeEnd;

	TextBlock = SNew(STextBlock)
		.Text(InArgs._Text)
		.TextStyle(InArgs._TextStyle)
		.Font(InArgs._Font)
		.StrikeBrush(InArgs._StrikeBrush)
		.ColorAndOpacity(InArgs._ColorAndOpacity)
		.ShadowOffset(InArgs._ShadowOffset)
		.ShadowColorAndOpacity(InArgs._ShadowColorAndOpacity)
		.HighlightColor(InArgs._HighlightColor)
		.HighlightShape(InArgs._HighlightShape)
		.HighlightText(InArgs._HighlightText)
		.WrapTextAt(InArgs._WrapTextAt)
		.AutoWrapText(InArgs._AutoWrapText)
		.WrappingPolicy(InArgs._WrappingPolicy)
		.Margin(InArgs._Margin)
		.LineHeightPercentage(InArgs._LineHeightPercentage)
		.Justification(InArgs._Justification)
		.MinDesiredWidth(InArgs._MinDesiredWidth)
		.TextShapingMethod(InArgs._TextShapingMethod)
		.TextFlowDirection(InArgs._TextFlowDirection)
		.LineBreakPolicy(InArgs._LineBreakPolicy)
		.SimpleTextMode(InArgs._SimpleTextMode)
		.OnDoubleClicked(InArgs._OnDoubleClicked)
		.Clipping(EWidgetClipping::Inherit)
		;

	SizeBox = SNew(SBox)
		.HAlign(InArgs._HAlign)
		.VAlign(InArgs._VAlign)
		.WidthOverride(InArgs._WidthOverride)
		.HeightOverride(InArgs._HeightOverride)
		[ TextBlock.ToSharedRef() ];

	ChildSlot.Padding(0, 0)
	[
		SizeBox.ToSharedRef()
	];
}

void SAdaptText::SetText(const TAttribute< FText >& InText)
{
	/*if (InText.IsSet() && !InText.IsBound())
	{
		TextBlock->SetText(InText);
	}*/
	BoundText = InText;
	Initialize();
}

void SAdaptText::SetText(const FText& InText)
{
	//TextBlock->SetText(InText);
	BoundText = InText;
	Initialize();
}

void SAdaptText::SetHighlightText(TAttribute<FText> InText)
{
	TextBlock->SetHighlightText(InText);
}

void SAdaptText::SetFont(const TAttribute<FSlateFontInfo>& InFont)
{
	if (!Font.IsSet() || !Font.IdenticalTo(InFont))
	{
		//TextBlock->SetFont(InFont);
		Font = InFont;
		Initialize();
		CurFontSize = MaxFontSize;
	}
}

void SAdaptText::SetStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush)
{
	TextBlock->SetStrikeBrush(InStrikeBrush);
}

void SAdaptText::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	TextBlock->SetColorAndOpacity(InColorAndOpacity);
}

void SAdaptText::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	TextBlock->SetTextStyle(InTextStyle);
}

void SAdaptText::SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	TextBlock->SetTextShapingMethod(InTextShapingMethod);
}

void SAdaptText::SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	TextBlock->SetTextFlowDirection(InTextFlowDirection);
}

void SAdaptText::SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	if (!WrapTextAt.IdenticalTo(InWrapTextAt))
	{
		WrapTextAt = InWrapTextAt;
		TextBlock->SetWrapTextAt(InWrapTextAt);
		Initialize();
	}
}

void SAdaptText::SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	if (!AutoWrapText.IdenticalTo(InAutoWrapText))
	{
		TextBlock->SetAutoWrapText(InAutoWrapText);
		AutoWrapText = InAutoWrapText;
		Initialize();
	}
}

void SAdaptText::SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	if (!WrappingPolicy.IdenticalTo(InWrappingPolicy))
	{
		TextBlock->SetWrappingPolicy(InWrappingPolicy);
		WrappingPolicy = InWrappingPolicy;
		Initialize();
	}
}

void SAdaptText::SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset)
{
	TextBlock->SetShadowOffset(InShadowOffset);
}

void SAdaptText::SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity)
{
	TextBlock->SetShadowColorAndOpacity(InShadowColorAndOpacity);
}

void SAdaptText::SetMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth)
{
	TextBlock->SetMinDesiredWidth(InMinDesiredWidth);
}

void SAdaptText::SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	TextBlock->SetLineHeightPercentage(InLineHeightPercentage);
}

void SAdaptText::SetMargin(const TAttribute<FMargin>& InMargin)
{
	TextBlock->SetMargin(InMargin);
}

void SAdaptText::SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	TextBlock->SetJustification(InJustification);
}

//SizeBox
void SAdaptText::SetHAlign(EHorizontalAlignment InHAlign)
{
	SizeBox->SetHAlign(InHAlign);
}

void SAdaptText::SetVAlign(EVerticalAlignment InVAlign)
{
	SizeBox->SetVAlign(InVAlign);
}

void SAdaptText::SetWidthOverride(TAttribute<FOptionalSize> InWidthOverride)
{
	SizeBox->SetWidthOverride(InWidthOverride);
}

void SAdaptText::SetHeightOverride(TAttribute<FOptionalSize> InHeightOverride)
{
	SizeBox->SetHeightOverride(InHeightOverride);
}

//SAdapter Text
void SAdaptText::SetTextAdaptType(const TAttribute<ETextAdaptType> InTextAdaptType)
{
	if (InTextAdaptType.IsSet() && !TextAdaptType.IdenticalTo(InTextAdaptType))
	{
		SetTextAdaptType(InTextAdaptType.Get());
	}
}

void SAdaptText::SetTextAdaptType(const ETextAdaptType InTextAdaptType)
{
	if (GetAdaptType() != InTextAdaptType)
	{
		TextAdaptType = InTextAdaptType;
		Initialize();
		IsFirstTimeToMarquee = true;
	}
}

void SAdaptText::SetTextMinFontSize(const int32 InMinFontSize)
{
	if (MinFontSize != InMinFontSize)
	{
		MinFontSize = InMinFontSize;
		Initialize();
	}
}

void SAdaptText::SetTextMarqueeDirection(const TAttribute<ETextMarqueeDirection> InTextMarqueeDireciton)
{
	if (InTextMarqueeDireciton.IsSet() && !InTextMarqueeDireciton.IsBound())
	{
		SetTextMarqueeDirection(InTextMarqueeDireciton.Get());
	}
}

void SAdaptText::SetTextMarqueeDirection(const ETextMarqueeDirection InTextMarqueeDireciton)
{
	if (GetMarqueeDirection() != InTextMarqueeDireciton)
	{
		TextMarqueeDirection = InTextMarqueeDireciton;
		Initialize();
		IsFirstTimeToMarquee = true;
	}
}

void SAdaptText::SetTextMarqueePolicy(const TAttribute<ETextMarqueePolicy> InTextMarqueePolicy)
{
	if (!TextMarqueePolicy.IdenticalTo(InTextMarqueePolicy))
	{
		TextMarqueePolicy = InTextMarqueePolicy;
		Initialize();
		IsFirstTimeToMarquee = true;
	}
}

void SAdaptText::SetTextMarqueeSpeed(const int32 InMarqueeSpeed)
{
	MarqueeSpeed = InMarqueeSpeed < 0 ? 0 : InMarqueeSpeed;
}

void SAdaptText::Initialize()
{
	//UE_LOG(LogTemp, Warning, TEXT("Initialize"));
	IsOnStart = true;
	IsFirstTimeToBestFit = true;
	TextStr = GetText().ToString();
	LastReduceTextSize = FVector2D::ZeroVector;
	LastIncreaseBoxSize = FVector2D::ZeroVector;

	SBoxPadding = FMargin();
	SizeBox->SetPadding(SBoxPadding);

	TextLength = TextStr.Len();
	EllipsisIndex = TextLength;

	CurFont = GetFont();
	MaxFontSize = CurFont.Size;
	CurFontSize = MaxFontSize;
	TextBlock->SetFont(CurFont);
	TextBlock->SetText(BoundText);
	
	TextBlock->SetRenderOpacity(0);
}

void SAdaptText::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	FVector2D BoxSize = AllottedGeometry.GetLocalSize();
	FVector2D TextSize = TextBlock->GetDesiredSize();
	switch (GetAdaptType())
	{
	case ETextAdaptType::Ellipsis:
		if (IsOnStart)
		{
			UE_LOG(LogTemp, Warning, TEXT("IsOnStart"));
			IsOnStart = false;
			break;
		}
		TickEllipsis(BoxSize, TextSize, InDeltaTime);
		while (IsFirstTimeToBestFit)
		{
			TextSize = TextBlock->ComputeDesiredSize(1);
			TickEllipsis(BoxSize, TextSize, InDeltaTime);
		}
		break;
	case ETextAdaptType::BestFit:
		if (IsOnStart)
		{
			IsOnStart = false;
			break;
		}
		TickBestFit(BoxSize, TextSize, InDeltaTime);
		/*while (IsFirstTimeToBestFit)
		{
			UE_LOG(LogTemp, Warning, TEXT("循环计算 BoxSize:%s  TextSize:%s"), *BoxSize.ToString(), *TextSize.ToString());
			TextSize = TextBlock->ComputeDesiredSize(1);
			TickBestFit(BoxSize, TextSize, InDeltaTime);
		}*/
		break;
	case ETextAdaptType::Marquee:
		TickMarquee(BoxSize, TextSize, InDeltaTime);
		break;
	}
}

bool SAdaptText::GetIsWrapText() const
{
	return AutoWrapText.Get() ? true : WrapTextAt.Get() > 0 ? true : false;
}

FSlateFontInfo SAdaptText::GetFont() const
{
	return Font.IsSet() ? Font.Get() : TextStyle.Font;
}

ETextAdaptType SAdaptText::GetAdaptType() const
{
	return TextAdaptType.IsSet() ? TextAdaptType.Get() : ETextAdaptType::Ellipsis;
}

ETextMarqueeDirection SAdaptText::GetMarqueeDirection() const
{
	return TextMarqueeDirection.IsSet() ? TextMarqueeDirection.Get() : ETextMarqueeDirection::RightToLeft;
}

ETextMarqueePolicy SAdaptText::GetMarqueePolicy() const
{
	return TextMarqueePolicy.IsSet() ? TextMarqueePolicy.Get() : ETextMarqueePolicy::Always;
}

void SAdaptText::TickEllipsis(const FVector2D& BoxSize, const FVector2D& TextSize, const float InDeltaTime)
{
	bool IsAutoWrapText = GetIsWrapText();
	//UE_LOG(LogTemp, Warning, TEXT("BoxSize:%s   TextSize:%s"), *BoxSize.ToString(), *TextSize.ToString());
	if (IsFirstTimeToBestFit 
		&& ((((IsAutoWrapText && BoxSize.Y >= TextSize.Y) || (!IsAutoWrapText && BoxSize.X >= TextSize.X))&& (EllipsisIndex == TextLength || BestFitState == -1)) || ((TextSize.X >= BoxSize.X || TextSize.Y >= BoxSize.Y) && EllipsisIndex == 0)))
	{
		TextBlock->SetRenderOpacity(1);
		IsFirstTimeToBestFit = false;
	}
	BestFitState = 0;
	if (IsAutoWrapText)
	{
		if (BoxSize.Y > TextSize.Y && EllipsisIndex < TextLength && (BoxSize.X - LastIncreaseBoxSize.X > 3 || (BoxSize.X == LastIncreaseBoxSize.X && BoxSize.Y >= LastReduceTextSize.Y)))
		{
			LastReduceTextSize = FVector2D::ZeroVector;
			LastIncreaseBoxSize = BoxSize;
			EllipsisIndex++;
			FString InString = TextStr.Left(EllipsisIndex);
			if (EllipsisIndex != TextLength)
			{
				InString += TEXT("...");
			}
			TextBlock->SetText(FText::AsCultureInvariant(InString));
			BestFitState = 1;
		}
		else if (TextSize.Y > BoxSize.Y && EllipsisIndex > 0)
		{
			LastReduceTextSize = TextSize;
			LastIncreaseBoxSize = BoxSize;
			EllipsisIndex--;
			FString InString = TextStr.Left(EllipsisIndex) + TEXT("...");
			TextBlock->SetText(FText::AsCultureInvariant(InString));
			BestFitState = -1;
		}
	}
	else
	{
		if (BoxSize.X > TextSize.X && EllipsisIndex < TextLength && BoxSize.X >= LastReduceTextSize.X)
		{
			EllipsisIndex++;
			FString InString = TextStr.Left(EllipsisIndex);
			if (EllipsisIndex != TextLength)
			{
				InString += TEXT("...");
			}
			TextBlock->SetText(FText::AsCultureInvariant(InString));
			BestFitState = 1;
		}
		else if (TextSize.X > BoxSize.X && EllipsisIndex > 0)
		{
			LastReduceTextSize = TextSize;
			EllipsisIndex -- ;
			FString InString = TextStr.Left(EllipsisIndex) + TEXT("...");
			TextBlock->SetText(FText::AsCultureInvariant(InString));
			BestFitState = -1;
		}
	}
}

void SAdaptText::TickBestFit(const FVector2D& BoxSize, const FVector2D& TextSize, const float InDeltaTime)
{
	if (IsFirstTimeToBestFit && ((BoxSize >= TextSize && ( CurFontSize == MaxFontSize || BestFitState == -1)) || ((TextSize.X >= BoxSize.X || TextSize.Y >= BoxSize.Y) && CurFontSize == MinFontSize)))
	{
		TextBlock->SetRenderOpacity(1);
		IsFirstTimeToBestFit = false;
		OnTextFontSizeChangedCallback.ExecuteIfBound(CurFontSize);
	}
	BestFitState = 0;
	if (GetIsWrapText())
	{
		if (BoxSize.Y > TextSize.Y && CurFontSize < MaxFontSize && (BoxSize.X - LastIncreaseBoxSize.X > 3 || (BoxSize.X <= LastIncreaseBoxSize.X && BoxSize.Y >= LastReduceTextSize.Y)))
		{
			LastReduceTextSize = FVector2D::ZeroVector;
			LastIncreaseBoxSize = BoxSize;
			CurFontSize++;
			CurFont.Size = CurFontSize;
			TextBlock->SetFont(CurFont);
			BestFitState = 1;
			if (!IsFirstTimeToBestFit)
			{
				OnTextFontSizeChangedCallback.ExecuteIfBound(CurFontSize);
			}
		}
		else if (TextSize.Y > BoxSize.Y && CurFontSize > MinFontSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("CurFontSize:%d"), CurFontSize);
			LastReduceTextSize = TextSize;
			LastIncreaseBoxSize = BoxSize;
			CurFontSize--;
			CurFont.Size = CurFontSize;
			TextBlock->SetFont(CurFont);
			BestFitState = -1;
			if (!IsFirstTimeToBestFit)
			{
				OnTextFontSizeChangedCallback.ExecuteIfBound(CurFontSize);
			}
		}
	}
	else
	{
		if (BoxSize > TextSize && CurFontSize < MaxFontSize && BoxSize >= LastReduceTextSize)
		{
			CurFontSize++;
			CurFont.Size = CurFontSize;
			TextBlock->SetFont(CurFont);
			BestFitState = 1;
			if (!IsFirstTimeToBestFit)
			{
				OnTextFontSizeChangedCallback.ExecuteIfBound(CurFontSize);
			}
		}
		else if ((TextSize.X > BoxSize.X || TextSize.Y > BoxSize.Y) && CurFontSize > MinFontSize)
		{
			//UE_LOG(LogTemp, Warning, TEXT("BoxSize:%s   TextSize:%s  CurFontSize:%d"), *BoxSize.ToString(), *TextSize.ToString(), CurFontSize);
			LastReduceTextSize = TextSize;
			CurFontSize--;
			CurFont.Size = CurFontSize;
			TextBlock->SetFont(CurFont);
			BestFitState = -1;
			if (!IsFirstTimeToBestFit)
			{
				OnTextFontSizeChangedCallback.ExecuteIfBound(CurFontSize);
			}
		}
	}
}

void SAdaptText::TickMarquee(const FVector2D& BoxSize, const FVector2D& TextSize, const float InDeltaTime)
{
	if (IsFirstTimeToBestFit)
	{
		IsFirstTimeToBestFit = false;
		TextBlock->SetRenderOpacity(1);
	}
	if (IsFirstTimeToMarquee)
	{
		UE_LOG(LogTemp, Warning, TEXT("执行TickMarquee"));
		IsFirstTimeToMarquee = false;
		switch (GetMarqueeDirection())
		{
		case ETextMarqueeDirection::LeftToRight: MarqueeOffset = -TextSize.X; break;
		case ETextMarqueeDirection::RightToLeft: MarqueeOffset = BoxSize.X; break;
		case ETextMarqueeDirection::TopToBottom: MarqueeOffset = -TextSize.Y; break;
		case ETextMarqueeDirection::BottomToTop: MarqueeOffset = BoxSize.Y; break;
		}
	}

	switch (GetMarqueeDirection())
	{
	case ETextMarqueeDirection::LeftToRight:
	{
		if (!ShouldMarquee(BoxSize, TextSize))
		{
			break;
		}
		if (MarqueeOffset > BoxSize.X)
		{
			MarqueeOffset = -TextSize.X;
			OnTextMarqueeEndCallback.ExecuteIfBound();
		}
		MarqueeOffset += MarqueeSpeed * InDeltaTime;
		SBoxPadding.Left = MarqueeOffset;
		SBoxPadding.Right = -MarqueeOffset;
		break;
	}
	case ETextMarqueeDirection::RightToLeft:
	{
		if (!ShouldMarquee(BoxSize, TextSize))
		{
			break;
		}
		if (MarqueeOffset < -TextSize.X)
		{
			MarqueeOffset = BoxSize.X;
			OnTextMarqueeEndCallback.ExecuteIfBound();
		}
		MarqueeOffset -= MarqueeSpeed * InDeltaTime;
		SBoxPadding.Left = MarqueeOffset;
		SBoxPadding.Right = -MarqueeOffset;
		break;
	}
	case ETextMarqueeDirection::TopToBottom:
	{
		if (!ShouldMarquee(BoxSize, TextSize))
		{
			break;
		}
		if (MarqueeOffset > BoxSize.Y)
		{
			MarqueeOffset = -TextSize.Y;
			OnTextMarqueeEndCallback.ExecuteIfBound();
		}
		MarqueeOffset += MarqueeSpeed * InDeltaTime;
		SBoxPadding.Top = MarqueeOffset;
		SBoxPadding.Bottom = -MarqueeOffset;
		break;
	}
	case ETextMarqueeDirection::BottomToTop:
	{
		if (!ShouldMarquee(BoxSize, TextSize))
		{
			break;
		}
		if (MarqueeOffset < -TextSize.Y)
		{
			MarqueeOffset = BoxSize.Y;
			OnTextMarqueeEndCallback.ExecuteIfBound();
		}
		MarqueeOffset -= MarqueeSpeed * InDeltaTime;
		SBoxPadding.Top = MarqueeOffset;
		SBoxPadding.Bottom = -MarqueeOffset;
		break;
	}
	}

	SizeBox->SetPadding(SBoxPadding);
}

bool SAdaptText::ShouldMarquee(const FVector2D& BoxSize, const FVector2D& TextSize)
{
	ETextMarqueePolicy Policy = GetMarqueePolicy();
	if ((Policy == ETextMarqueePolicy::OverflowX && TextSize.X <= BoxSize.X)
		|| (Policy == ETextMarqueePolicy::OverflowY && TextSize.Y <= BoxSize.Y)
		|| (Policy== ETextMarqueePolicy::OverflowBoth && (TextSize.X <= BoxSize.X || TextSize.Y <= BoxSize.Y)))
	{
		MarqueeOffset = 0;
		SBoxPadding = FMargin();
		return false;
	}

	return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION