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
#include "SAdaptTextBase.h"

UENUM(BlueprintType)
enum class ETextAdaptType : uint8 { Ellipsis, BestFit, Marquee };
UENUM(BlueprintType)
enum class ETextMarqueeType : uint8 { Default, DelayStart };
UENUM(BlueprintType)
enum class ETextMarqueeDirection : uint8 { LeftToRight, RightToLeft, TopToBottom, BottomToTop };
UENUM(BlueprintType)
enum class ETextMarqueePolicy : uint8 { Always, OverflowX, OverflowY, OverflowBoth };

DECLARE_DELEGATE_OneParam(FOnTextFontSizeChanged, int32);
DECLARE_DELEGATE(FOnTextMarqueeEnd);

/**
 *
 */
class FIRSTGAME_API SAdaptText : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAdaptText)
		//STextBlock's properties
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
		, _TextMinDesiredWidth(0.0f)
		, _TextShapingMethod()
		, _TextFlowDirection()
		, _LineBreakPolicy()
		, _SimpleTextMode(false)
		//SizeBox's properties
		, _HAlign(HAlign_Fill)
		, _VAlign(VAlign_Fill)
		, _WidthOverride(FOptionalSize())
		, _HeightOverride(FOptionalSize())
		, _MinDesiredWidth(FOptionalSize())
		, _MinDesiredHeight(FOptionalSize())
		, _MaxDesiredWidth(FOptionalSize())
		, _MaxDesiredHeight(FOptionalSize())
		//SAdaptText's properties
		, _TextAdaptType(ETextAdaptType::Ellipsis)
		, _TextMinFontSize(0)
		, _TextMarqueeType(ETextMarqueeType::Default)
		, _TextMarqueeDirection(ETextMarqueeDirection::LeftToRight)
		, _TextMarqueePolicy(ETextMarqueePolicy::Always)
		, _TextMarqueeSpeed(0)
		, _TextMarqueeStartDelay(0.0f)
		, _TextMarqueeStartDelayOnlyFirst(true)
	{
		_Clipping = EWidgetClipping::Inherit;
		_Visibility = EVisibility::SelfHitTestInvisible;
	}

	/**************************** STextBlock ********************************/

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
		SLATE_ATTRIBUTE(float, TextMinDesiredWidth)

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

		/**************************** SizeBox ********************************/

		/** Horizontal alignment of content in the area allotted to the SBox by its parent */
		SLATE_ARGUMENT(EHorizontalAlignment, HAlign)

		/** Vertical alignment of content in the area allotted to the SBox by its parent */
		SLATE_ARGUMENT(EVerticalAlignment, VAlign)

		/** When specified, ignore the content's desired size and report the WidthOverride as the Box's desired width. */
		SLATE_ATTRIBUTE(FOptionalSize, WidthOverride)

		/** When specified, ignore the content's desired size and report the HeightOverride as the Box's desired height. */
		SLATE_ATTRIBUTE(FOptionalSize, HeightOverride)

		/** When specified, will report the MinDesiredWidth if larger than the content's desired width. */
		SLATE_ATTRIBUTE(FOptionalSize, MinDesiredWidth)

		/** When specified, will report the MinDesiredHeight if larger than the content's desired height. */
		SLATE_ATTRIBUTE(FOptionalSize, MinDesiredHeight)

		/** When specified, will report the MaxDesiredWidth if smaller than the content's desired width. */
		SLATE_ATTRIBUTE(FOptionalSize, MaxDesiredWidth)

		/** When specified, will report the MaxDesiredHeight if smaller than the content's desired height. */
		SLATE_ATTRIBUTE(FOptionalSize, MaxDesiredHeight)

		/**************************** SAdaptText ********************************/

		/** �ı���Ӧ���� */
		SLATE_ATTRIBUTE(ETextAdaptType, TextAdaptType)

		/** ��С���ֺ� */
		SLATE_ATTRIBUTE(int32, TextMinFontSize)

		/** BestFit�»�ı��ֺţ�����֪ͨһ������ */
		SLATE_EVENT(FOnTextFontSizeChanged, OnTextFontSizeChanged)

		/** �������� */
		SLATE_ATTRIBUTE(ETextMarqueeType, TextMarqueeType)

		/** �ı��������� */
		SLATE_ATTRIBUTE(ETextMarqueeDirection, TextMarqueeDirection)

		/** �ı��������� */
		SLATE_ATTRIBUTE(ETextMarqueePolicy, TextMarqueePolicy)

		/** �ı������ٶ� */
		SLATE_ATTRIBUTE(int32, TextMarqueeSpeed)

		/** ��ʱ���� */
		SLATE_ATTRIBUTE(float, TextMarqueeStartDelay)

		/** ����һ��ʱ��ʱ���� */
		SLATE_ATTRIBUTE(bool, TextMarqueeStartDelayOnlyFirst)

		/** �ı�����һ�� */
		SLATE_EVENT(FOnTextMarqueeEnd, OnTextMarqueeEnd)

		SLATE_END_ARGS()

public:
	/** Constructor */
	SAdaptText();

	/** Destructor */
	~SAdaptText();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/**
	 * Gets the text assigned to this text block
	 *
	 * @return	This text block's text string
	 */
	const FText& GetText() const;

	/**
	 * ��ȡ��ǰ�ֺ�
	 *
	 * @return �ֺŴ�С
	 */
	const int32 GetCurrentFontSize() const;

public:

	/**************************** STextBlock ********************************/
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
	void SetHighlightText(TAttribute<FText> InHighlightText);

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
	void SetTextMinDesiredWidth(const TAttribute<float>& InTextMinDesiredWidth);

	/** See LineHeightPercentage attribute */
	void SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage);

	/** See Margin attribute */
	void SetMargin(const TAttribute<FMargin>& InMargin);

	/** See Justification attribute */
	void SetJustification(const TAttribute<ETextJustify::Type>& InJustification);


	/**************************** SizeBox ********************************/

	/**Set SizeBox slot*/
	void SetHAlign(EHorizontalAlignment InHAlign);

	/**Set SizeBox slot*/
	void SetVAlign(EVerticalAlignment InVAlign);

	/** See WidthOverride attribute */
	void SetWidthOverride(TAttribute<FOptionalSize> InWidthOverride);

	/** See HeightOverride attribute */
	void SetHeightOverride(TAttribute<FOptionalSize> InHeightOverride);

	/** See MinDesiredWidth attribute */
	void SetMinDesiredWidth(TAttribute < FOptionalSize> InMinDesiredWidth);

	/** See MinDesiredHeight attribute */
	void SetMinDesiredHeight(TAttribute<FOptionalSize> InMinDesiredHeight);

	/** See MaxDesiredWidth attribute */
	void SetMaxDesiredWidth(TAttribute<FOptionalSize> InMaxDesiredWidth);

	/** See MaxDesiredHeight attribute */
	void SetMaxDesiredHeight(TAttribute<FOptionalSize> InMaxDesiredHeight);


	/**************************** SAdaptText ********************************/
	/**Set text adapting type*/
	void SetTextAdaptType(const TAttribute<ETextAdaptType>& InTextAdaptType);

	/**Set text min font size*/
	void SetTextMinFontSize(const TAttribute<int32>& InTextMinFontSize);

	/**Set text marquee direction*/
	void SetTextMarqueeDirection(const TAttribute<ETextMarqueeDirection>& InTextMarqueeDireciton);

	/**Set text marquee policy*/
	void SetTextMarqueePolicy(const TAttribute<ETextMarqueePolicy>& InTextMarqueePolicy);

	/**Set text marquee speed*/
	void SetTextMarqueeSpeed(const TAttribute<int32>& InTextMarqueeSpeed);

	void SetTextMarqueeType(const TAttribute<ETextMarqueeType>& InTextMarqueeType);

	void SetTextMarqueeStartDelay(const TAttribute<float>& InTextMarqueeStartDelay);

	void SetTextMarqueeStartDelayOnlyFirst(const TAttribute<bool>& InTextMarqueeStartDelayOnlyFirst);


	/**************************** Widget ********************************/

	/**Tick*/
	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	void BuildChildWidget();

private:

	FArguments CacheArgs;

	TSharedPtr<FAdaptTextBase> MyAdaptText;

	ETextAdaptType AdaptType;
};
