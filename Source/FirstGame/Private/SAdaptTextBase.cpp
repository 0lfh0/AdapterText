#include "SAdaptTextBase.h"

/**************************** STextBlock ********************************/
void FAdaptTextBase::SetText(const TAttribute< FText >& InText)
{
	GetTextBlock()->SetText(InText);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetText);
}

void FAdaptTextBase::SetText(const FText& InText)
{
	GetTextBlock()->SetText(InText);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetText);
}

void FAdaptTextBase::SetHighlightText(TAttribute<FText> InText)
{
	GetTextBlock()->SetHighlightText(InText);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetHighlightText);
}

void FAdaptTextBase::SetFont(const TAttribute< FSlateFontInfo >& InFont)
{
	GetTextBlock()->SetFont(InFont);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetFont);
}

void FAdaptTextBase::SetStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush)
{
	GetTextBlock()->SetStrikeBrush(InStrikeBrush);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetStrikeBrush);
}

void FAdaptTextBase::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	GetTextBlock()->SetColorAndOpacity(InColorAndOpacity);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetColorAndOpacity);
}

void FAdaptTextBase::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	GetTextBlock()->SetTextStyle(InTextStyle);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetTextStyle);
}

void FAdaptTextBase::SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	GetTextBlock()->SetTextShapingMethod(InTextShapingMethod);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetTextShapingMethod);
}

void FAdaptTextBase::SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	GetTextBlock()->SetTextFlowDirection(InTextFlowDirection);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetTextFlowDirection);
}

void FAdaptTextBase::SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	GetTextBlock()->SetWrapTextAt(InWrapTextAt);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetWrapTextAt);
}

void FAdaptTextBase::SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	GetTextBlock()->SetAutoWrapText(InAutoWrapText);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetAutoWrapText);
}

void FAdaptTextBase::SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	GetTextBlock()->SetWrappingPolicy(InWrappingPolicy);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetWrappingPolicy);
}

void FAdaptTextBase::SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset)
{
	GetTextBlock()->SetShadowOffset(InShadowOffset);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetShadowOffset);
}

void FAdaptTextBase::SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity)
{
	GetTextBlock()->SetShadowColorAndOpacity(InShadowColorAndOpacity);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetShadowColorAndOpacity);
}

void FAdaptTextBase::SetTextMinDesiredWidth(const TAttribute<float>& InTextMinDesiredWidth)
{
	GetTextBlock()->SetMinDesiredWidth(InTextMinDesiredWidth);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetTextMinDesiredWidth);
}

void FAdaptTextBase::SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	GetTextBlock()->SetLineHeightPercentage(InLineHeightPercentage);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetLineHeightPercentage);
}

void FAdaptTextBase::SetMargin(const TAttribute<FMargin>& InMargin)
{
	GetTextBlock()->SetMargin(InMargin);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetMargin);
}

void FAdaptTextBase::SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	GetTextBlock()->SetJustification(InJustification);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetJustification);
}

/**************************** SizeBox ********************************/

void FAdaptTextBase::SetHAlign(EHorizontalAlignment InHAlign)
{
	GetSizeBox()->SetHAlign(InHAlign);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetHAlign);
}

void FAdaptTextBase::SetVAlign(EVerticalAlignment InVAlign)
{
	GetSizeBox()->SetVAlign(InVAlign);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetVAlign);
}

void FAdaptTextBase::SetWidthOverride(TAttribute<FOptionalSize> InWidthOverride)
{
	GetSizeBox()->SetWidthOverride(InWidthOverride);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetWidthOverride);
}

void FAdaptTextBase::SetHeightOverride(TAttribute<FOptionalSize> InHeightOverride)
{
	GetSizeBox()->SetHeightOverride(InHeightOverride);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetHeightOverride);
}

void FAdaptTextBase::SetMinDesiredWidth(TAttribute < FOptionalSize> InMinDesiredWidth)
{
	GetSizeBox()->SetMinDesiredWidth(InMinDesiredWidth);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetMinDesiredWidth);
}

void FAdaptTextBase::SetMinDesiredHeight(TAttribute<FOptionalSize> InMinDesiredHeight)
{
	GetSizeBox()->SetMinDesiredHeight(InMinDesiredHeight);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetMinDesiredHeight);
}

void FAdaptTextBase::SetMaxDesiredWidth(TAttribute<FOptionalSize> InMaxDesiredWidth)
{
	GetSizeBox()->SetMaxDesiredWidth(InMaxDesiredWidth);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetMaxDesiredWidth);
}

void FAdaptTextBase::SetMaxDesiredHeight(TAttribute<FOptionalSize> InMaxDesiredHeight)
{
	GetSizeBox()->SetMaxDesiredHeight(InMaxDesiredHeight);
	InvalidateAdaptText(EInvalidateAdaptTextReason::SetMaxDesiredHeight);
}

void FAdaptTextBase::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (InvalidateReason != 0)
	{
		OnInvalidate(AllottedGeometry, InCurrentTime, InDeltaTime, (EInvalidateAdaptTextReason)InvalidateReason);
		InvalidateReason = 0;
	}
	OnTick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void FAdaptTextBase::InvalidateAdaptText(EInvalidateAdaptTextReason Reason)
{
	InvalidateReason |= (uint32)Reason;
}

bool FAdaptTextBase::HasInvalidateFlags(EInvalidateAdaptTextReason Reason)
{
	return (InvalidateReason & ((uint32)Reason)) != 0;
}
