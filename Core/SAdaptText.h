// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Misc/Attribute.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/SlateColor.h"
#include "Input/Reply.h"
#include "Layout/Margin.h"
#include "Styling/SlateWidgetStyleAsset.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Framework/SlateDelegates.h"
#include "Framework/Text/TextLayout.h"

UENUM(BlueprintType)
enum class ETextAdaptType: uint8 { Ellipsis, BestFit, Marquee};
UENUM(BlueprintType)
enum class ETextMarqueeDirection: uint8 { LeftToRight, RightToLeft, TopToBottom, BottomToTop };
UENUM(BlueprintType)
enum class ETextMarqueePolicy: uint8 { Always, OverflowX, OverflowY, OverflowBoth };
DECLARE_DELEGATE_OneParam(FOnTextFontSizeChanged, int32);
DECLARE_DELEGATE(FOnTextMarqueeEnd);
/**
 * 
 */
class TESTGAME_API SAdaptText : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdaptText)
		: _Text()
		, _TextStyle(&FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
		, _Font()
		, _StrikeBrush()
		, _ColorAndOpacity()
		, _ShadowOffset()
		, _ShadowColorAndOpacity()
		, _HighlightColor()
		, _HighlightShape()
		, _HighlightText()
		, _WrapTextAt(0.0f)
		, _AutoWrapText(false)
		, _WrappingPolicy(ETextWrappingPolicy::DefaultWrapping)
		, _Margin()
		, _LineHeightPercentage(1.0f)
		, _Justification(ETextJustify::Left)
		, _MinDesiredWidth(0.0f)
		, _TextShapingMethod()
		, _TextFlowDirection()
		, _LineBreakPolicy()
		, _SimpleTextMode(false)
		, _TextAdaptType(ETextAdaptType::Ellipsis)
		, _TextMinFontSize(0)
		, _TextMarqueeDirection(ETextMarqueeDirection::LeftToRight)
		, _TextMarqueePolicy(ETextMarqueePolicy::Always)
		, _TextMarqueeSpeed(0)
		, _HAlign(HAlign_Fill)
		, _VAlign(VAlign_Fill)
		, _WidthOverride(FOptionalSize())
		, _HeightOverride(FOptionalSize())
		{
			_Clipping = EWidgetClipping::OnDemand;
		}

		/** The text displayed in this text block */
		SLATE_ATTRIBUTE(FText, Text)

		/** Pointer to a style of the text block, which dictates the font, color, and shadow options. */
		SLATE_STYLE_ARGUMENT(FTextBlockStyle, TextStyle)

		/** Sets the font used to draw the text */
		SLATE_ATTRIBUTE(FSlateFontInfo, Font)

		/** Sets the brush used to strike through the text */
		SLATE_ATTRIBUTE(const FSlateBrush*, StrikeBrush)

		/** Text color and opacity */
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

		/** Drop shadow offset in pixels */
		SLATE_ATTRIBUTE(FVector2D, ShadowOffset)

		/** Shadow color and opacity */
		SLATE_ATTRIBUTE(FLinearColor, ShadowColorAndOpacity)

		/** The color used to highlight the specified text */
		SLATE_ATTRIBUTE(FLinearColor, HighlightColor)

		/** The brush used to highlight the specified text*/
		SLATE_ATTRIBUTE(const FSlateBrush*, HighlightShape)

		/** Highlight this text in the text block */
		SLATE_ATTRIBUTE(FText, HighlightText)

		/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no wrapping occurs. */
		SLATE_ATTRIBUTE(float, WrapTextAt)

		/** Whether to wrap text automatically based on the widget's computed horizontal space.  IMPORTANT: Using automatic wrapping can result
			in visual artifacts, as the the wrapped size will computed be at least one frame late!  Consider using WrapTextAt instead.  The initial
			desired size will not be clamped.  This works best in cases where the text block's size is not affecting other widget's layout. */
		SLATE_ATTRIBUTE(bool, AutoWrapText)

		/** The wrapping policy to use */
		SLATE_ATTRIBUTE(ETextWrappingPolicy, WrappingPolicy)

		/** The amount of blank space left around the edges of text area. */
		SLATE_ATTRIBUTE(FMargin, Margin)

		/** The amount to scale each lines height by. */
		SLATE_ATTRIBUTE(float, LineHeightPercentage)

		/** How the text should be aligned with the margin. */
		SLATE_ATTRIBUTE(ETextJustify::Type, Justification)

		/** Minimum width that a text block should be */
		SLATE_ATTRIBUTE(float, MinDesiredWidth)

		/** Which text shaping method should we use? (unset to use the default returned by GetDefaultTextShapingMethod) */
		SLATE_ARGUMENT(TOptional<ETextShapingMethod>, TextShapingMethod)

		/** Which text flow direction should we use? (unset to use the default returned by GetDefaultTextFlowDirection) */
		SLATE_ARGUMENT(TOptional<ETextFlowDirection>, TextFlowDirection)

		/** The iterator to use to detect appropriate soft-wrapping points for lines (or null to use the default) */
		SLATE_ARGUMENT(TSharedPtr<IBreakIterator>, LineBreakPolicy)

		/**
		 * If this is enabled, text shaping, wrapping, justification are disabled in favor of much faster text layout and measurement.
		 * This feature is suitable for numbers and text that changes often and impact performance.
		 * Enabling this setting may cause certain languages (such as Right to left languages) to not display properly.
		 */
		SLATE_ARGUMENT(bool, SimpleTextMode)

		/** Called when this text is double clicked */
		SLATE_EVENT(FPointerEventHandler, OnDoubleClicked)

		//SAdaptText
		/*文本适应类型*/
		SLATE_ATTRIBUTE(ETextAdaptType, TextAdaptType)
		/*最小的字号*/
		SLATE_ARGUMENT(int32, TextMinFontSize)
		/*BestFit下会改变字号，这里通知一下外面*/
		SLATE_EVENT(FOnTextFontSizeChanged, OnTextFontSizeChanged)
		/*文本滚动方向*/
		SLATE_ATTRIBUTE(ETextMarqueeDirection, TextMarqueeDirection)
		/*文本滚动策略*/
		SLATE_ATTRIBUTE(ETextMarqueePolicy, TextMarqueePolicy)
		/**文本滚动速度*/
		SLATE_ARGUMENT(int32, TextMarqueeSpeed)

		/** Horizontal alignment of content in the area allotted to the SBox by its parent */
		SLATE_ARGUMENT(EHorizontalAlignment, HAlign)

		/** Vertical alignment of content in the area allotted to the SBox by its parent */
		SLATE_ARGUMENT(EVerticalAlignment, VAlign)

		/** When specified, ignore the content's desired size and report the WidthOverride as the Box's desired width. */
		SLATE_ATTRIBUTE(FOptionalSize, WidthOverride)

		/** When specified, ignore the content's desired size and report the HeightOverride as the Box's desired height. */
		SLATE_ATTRIBUTE(FOptionalSize, HeightOverride)

		SLATE_EVENT(FOnTextMarqueeEnd, OnTextMarqueeEnd)

	SLATE_END_ARGS()

	/*Constructor*/
	SAdaptText();
	/*Destructor*/
	~SAdaptText();
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/**
	 * Gets the text assigned to this text block
	 *
	 * @return	This text block's text string
	 */
	const FText& GetText() const
	{
		return BoundText.Get();
	}

	/*获取当前字体大小*/
	const int32 GetCurrentFontSize() const
	{
		return CurFontSize;
	}

public:
	/**
	 * Sets the text for this text block
	 *
	 * @param	InText	The new text to display
	 */
	void SetText(const TAttribute< FText >& InText);
	void SetText(const FText& InText);

	/**
	* Sets the highlight text for this text block
	*
	* @param	InText	The new text to highlight
	*/
	void SetHighlightText(TAttribute<FText> InText);

	/**
	 * Sets the font used to draw the text
	 *
	 * @param	InFont	The new font to use
	 */
	void SetFont(const TAttribute< FSlateFontInfo >& InFont);

	/** Sets the brush used to strike through the text */
	void SetStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush);

	/** See ColorAndOpacity attribute */
	void SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity);

	/** See TextStyle argument */
	void SetTextStyle(const FTextBlockStyle* InTextStyle);

	/** See TextShapingMethod attribute */
	void SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod);

	/** See TextFlowDirection attribute */
	void SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection);

	/** See WrapTextAt attribute */
	void SetWrapTextAt(const TAttribute<float>& InWrapTextAt);

	/** See AutoWrapText attribute */
	void SetAutoWrapText(const TAttribute<bool>& InAutoWrapText);

	/** Set WrappingPolicy attribute */
	void SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy);

	/** See ShadowOffset attribute */
	void SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset);

	/** See ShadowColorAndOpacity attribute */
	void SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity);

	/** See MinDesiredWidth attribute */
	void SetMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth);

	/** See LineHeightPercentage attribute */
	void SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage);

	/** See Margin attribute */
	void SetMargin(const TAttribute<FMargin>& InMargin);

	/** See Justification attribute */
	void SetJustification(const TAttribute<ETextJustify::Type>& InJustification);

public:
	/**Set SizeBox slot*/
	void SetHAlign(EHorizontalAlignment InHAlign);

	/**Set SizeBox slot*/
	void SetVAlign(EVerticalAlignment InVAlign);

	/** See WidthOverride attribute */
	void SetWidthOverride(TAttribute<FOptionalSize> InWidthOverride);

	/** See HeightOverride attribute */
	void SetHeightOverride(TAttribute<FOptionalSize> InHeightOverride);

public:
	/**Set text adapting type*/
	void SetTextAdaptType(const TAttribute<ETextAdaptType> InTextAdaptType);

	/**Set text adapting type*/
	void SetTextAdaptType(const ETextAdaptType InTextAdaptType);

	/**Set text min font size*/
	void SetTextMinFontSize(const int32 InMinFontSize);

	/**Set text marquee direction*/
	void SetTextMarqueeDirection(const TAttribute<ETextMarqueeDirection> InTextMarqueeDireciton);

	/**Set text marquee direction*/
	void SetTextMarqueeDirection(const ETextMarqueeDirection InTextMarqueeDireciton);

	/**Set text marquee policy*/
	void SetTextMarqueePolicy(const TAttribute<ETextMarqueePolicy> InTextMarqueePolicy);

	/**Set text marquee speed*/
	void SetTextMarqueeSpeed(const int32 InMarqueeSpeed);

	/**Initialize*/
	void Initialize();

	/**Tick*/
	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	bool GetIsWrapText() const;
	FSlateFontInfo GetFont() const;
	ETextAdaptType GetAdaptType() const;
	ETextMarqueeDirection GetMarqueeDirection() const;
	ETextMarqueePolicy GetMarqueePolicy() const;

	void TickEllipsis(const FVector2D& BoxSize, const FVector2D& TextSize, const float InDeltaTime);
	void TickBestFit(const FVector2D& BoxSize, const FVector2D& TextSize, const float InDeltaTime);
	void TickMarquee(const FVector2D& BoxSize, const FVector2D& TextSize, const float InDeltaTime);
	bool ShouldMarquee(const FVector2D& BoxSize, const FVector2D& TextSize);

private:
	/** Default style used by the TextLayout */
	FTextBlockStyle TextStyle;

	/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no wrapping occurs. */
	TAttribute<float> WrapTextAt;

	/** True if we're wrapping text automatically based on the computed horizontal space for this widget */
	TAttribute<bool> AutoWrapText;

	/** The wrapping policy we're using */
	TAttribute<ETextWrappingPolicy> WrappingPolicy;

	/** Sets the font used to draw the text */
	TAttribute< FSlateFontInfo > Font;

	/** The text displayed in this text block */
	TAttribute< FText > BoundText;

private:
	/**Real text content*/
	TSharedPtr<STextBlock> TextBlock;
	/**Text container*/
	TSharedPtr<SBox> SizeBox;
	/**文本适应方式*/
	TAttribute<ETextAdaptType> TextAdaptType;
	/**文本滚动方向*/
	TAttribute<ETextMarqueeDirection> TextMarqueeDirection;
	/**文本滚动策略*/
	TAttribute<ETextMarqueePolicy> TextMarqueePolicy;
	bool IsOnStart;
	/**First time to fit*/
	bool IsFirstTimeToBestFit;
	/**FText to FString*/
	FString TextStr;
	FVector2D LastReduceTextSize;
	FVector2D LastIncreaseBoxSize;

	bool IsFirstTimeToMarquee = true;
	/**滚动偏移*/
	float MarqueeOffset;
	/**滚动速度*/
	int32 MarqueeSpeed;
	/**SizeBox 的padding*/
	FMargin SBoxPadding;
	FOnTextMarqueeEnd OnTextMarqueeEndCallback;

	/**Current text length*/
	int32 TextLength;
	/**Current ellipsis index*/
	int32 EllipsisIndex;

	/**最大字号*/
	int32 MaxFontSize;
	/**最小字号*/
	int32 MinFontSize;
	/**当前显示的字号*/
	int32 CurFontSize;
	/**当前显示的字体*/
	FSlateFontInfo CurFont;
	/**当字号改变时通知外面*/
	FOnTextFontSizeChanged OnTextFontSizeChangedCallback;
	int32 BestFitState;
};
