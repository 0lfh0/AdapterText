#pragma once

enum class EInvalidateAdaptTextReason : uint32
{
	SetText = 1 << 0,
	SetHighlightText = 1 << 1,
	SetFont = 1 << 2,
	SetStrikeBrush = 1 << 3,
	SetColorAndOpacity = 1 << 4,
	SetTextStyle = 1 << 5,
	SetTextShapingMethod = 1 << 6,
	SetTextFlowDirection = 1 << 7,
	SetWrapTextAt = 1 << 8,
	SetAutoWrapText = 1 << 9,
	SetWrappingPolicy = 1 << 10,
	SetShadowOffset = 1 << 11,
	SetShadowColorAndOpacity = 1 << 12,
	SetTextMinDesiredWidth = 1 << 13,
	SetLineHeightPercentage = 1 << 14,
	SetMargin = 1 << 15,
	SetJustification = 1 << 16,

	SetHAlign = 1 << 17,
	SetVAlign = 1 << 18,
	SetWidthOverride = 1 << 19,
	SetHeightOverride = 1 << 20,
	SetMinDesiredWidth = 1 << 21,
	SetMinDesiredHeight = 1 << 22,
	SetMaxDesiredWidth = 1 << 23,
	SetMaxDesiredHeight = 1 << 24,

	SetTextMarquee = 1 << 25,
};

class FAdaptTextBase
{
public:
	FAdaptTextBase()
	{

	}

	virtual ~FAdaptTextBase()
	{

	}

public:
	/**************************** STextBlock ********************************/
	virtual void SetText(const TAttribute< FText >& InText);
	virtual void SetText(const FText& InText);
	virtual void SetHighlightText(TAttribute<FText> InText);
	virtual void SetFont(const TAttribute< FSlateFontInfo >& InFont);
	virtual void SetStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush);
	virtual void SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity);
	virtual void SetTextStyle(const FTextBlockStyle* InTextStyle);
	virtual void SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod);
	virtual void SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection) ;
	virtual void SetWrapTextAt(const TAttribute<float>& InWrapTextAt);
	virtual void SetAutoWrapText(const TAttribute<bool>& InAutoWrapText) ;
	virtual void SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy);
	virtual void SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset);
	virtual void SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity);
	virtual void SetTextMinDesiredWidth(const TAttribute<float>& InTextMinDesiredWidth) ;
	virtual void SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage) ;
	virtual void SetMargin(const TAttribute<FMargin>& InMargin);
	virtual void SetJustification(const TAttribute<ETextJustify::Type>& InJustification);

	/**************************** SizeBox ********************************/
	virtual void SetHAlign(EHorizontalAlignment InHAlign);
	virtual void SetVAlign(EVerticalAlignment InVAlign);
	virtual void SetWidthOverride(TAttribute<FOptionalSize> InWidthOverride);
	virtual void SetHeightOverride(TAttribute<FOptionalSize> InHeightOverride) ;
	virtual void SetMinDesiredWidth(TAttribute < FOptionalSize> InMinDesiredWidth);
	virtual void SetMinDesiredHeight(TAttribute<FOptionalSize> InMinDesiredHeight);
	virtual void SetMaxDesiredWidth(TAttribute<FOptionalSize> InMaxDesiredWidth) ;
	virtual void SetMaxDesiredHeight(TAttribute<FOptionalSize> InMaxDesiredHeight);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

protected:
	void InvalidateAdaptText(EInvalidateAdaptTextReason Reason);
	bool HasInvalidateFlags(EInvalidateAdaptTextReason Reason);
	virtual TSharedRef<SBox> GetSizeBox() = 0;
	virtual TSharedRef<STextBlock> GetTextBlock() = 0;
	virtual void OnInvalidate(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime, const EInvalidateAdaptTextReason& Reason) = 0;
	virtual void OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) = 0;

public:
	virtual TSharedRef<SWidget> GetWidget() = 0;

private:
	uint32 InvalidateReason;

};