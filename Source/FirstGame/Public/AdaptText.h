// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"
#include "Widgets/SWidget.h"
#include "Components/TextWidgetTypes.h"
#include "SAdaptText.h"
#include "AdaptText.generated.h"
/**
 *
 */
UCLASS(meta = (DisplayName = "Race Adapter Text"))
class FIRSTGAME_API UAdaptText : public UTextLayoutWidget
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextFontSizeChangedEvent, const int32&, FontSize);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTextMarqueeEndEvent);

public:
	/**
	 * Sets the color and opacity of the text in this text block
	 *
	 * @param InColorAndOpacity		The new text color and opacity
	 */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetColorAndOpacity(FSlateColor InColorAndOpacity);

	/**
	* Sets the opacity of the text in this text block
	*
	* @param InOpacity		The new text opacity
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetOpacity(float InOpacity);

	/**
	 * Sets the color and opacity of the text drop shadow
	 * Note: if opacity is zero no shadow will be drawn
	 *
	 * @param InShadowColorAndOpacity		The new drop shadow color and opacity
	 */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetShadowColorAndOpacity(FLinearColor InShadowColorAndOpacity);

	/**
	 * Sets the offset that the text drop shadow should be drawn at
	 *
	 * @param InShadowOffset		The new offset
	 */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetShadowOffset(FVector2D InShadowOffset);

	/**
	 * Dynamically set the font info for this text block
	 *
	 * @param InFontInfo THe new font info
	 */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetFont(FSlateFontInfo InFontInfo);

	/**
	 * Dynamically set the strike brush for this text block
	 *
	 * @param InStrikeBrush The new brush to use to strike through text
	 */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetStrikeBrush(FSlateBrush InStrikeBrush);

	/**
	 *  Set the minimum desired width for this text block
	 *
	 *  @param InMinDesiredWidth new minimum desired width
	 */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetTextMinDesiredWidth(float InTextMinDesiredWidth);

	/**
	 * Set the auto wrap for this text block.
	 *
	 * @param InAutoTextWrap to turn wrap on or off.
	 */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetAutoWrapText(bool InAutoTextWrap);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		UMaterialInstanceDynamic* GetDynamicFontMaterial();

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		UMaterialInstanceDynamic* GetDynamicOutlineMaterial();

public:
	/** The text to display */
	UPROPERTY(EditAnywhere, Category = Content, meta = (MultiLine = "false"))
		FText Text;

	/** A bindable delegate to allow logic to drive the text of the widget */
	UPROPERTY()
		FGetText TextDelegate;

	/** The color of the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		FSlateColor ColorAndOpacity;

	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
		FGetSlateColor ColorAndOpacityDelegate;

	/** The font to render the text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		FSlateFontInfo Font;

	/** The brush to strike through text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		FSlateBrush StrikeBrush;

	/** The direction the shadow is cast */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		FVector2D ShadowOffset;

	/** The color of the shadow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (DisplayName = "Shadow Color"))
		FLinearColor ShadowColorAndOpacity;

	/** A bindable delegate for the ShadowColorAndOpacity. */
	UPROPERTY()
		FGetLinearColor ShadowColorAndOpacityDelegate;

	/** The minimum desired size for the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		float TextMinDesiredWidth;

	/** If true, it will automatically wrap this text widget with an invalidation panel */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Performance, AdvancedDisplay)
		bool bWrapWithInvalidationPanel;

	/** Whether the text should automatically wrap */
	UPROPERTY(meta = (DeprecatedProperty, DeprecationMessage = "bAutoWrapText is deprecated. Please use AutoWrapText instead."))
		bool bAutoWrapText_DEPRECATED;

	/**
	 * Gets the widget text
	 * @return The widget text
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "GetText (Adapter Text)"))
		FText GetText() const;

	/**
	 * Directly sets the widget text.
	 * Warning: This will wipe any binding created for the Text property!
	 * @param InText The text to assign to the widget
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "SetText (Adapter Text)"))
		virtual void SetText(FText InText);

	//~ Begin UTextLayoutWidget Interface
	virtual void SetJustification(ETextJustify::Type InJustification) override;
	//~ End UTextLayoutWidget Interface

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface
#if WITH_EDITOR
	//~ Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;
	virtual void OnCreationFromPalette() override;
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	//~ End UWidget Interface

	virtual FString GetLabelMetadata() const override;

	void HandleTextCommitted(const FText& InText, ETextCommit::Type CommitteType);
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetAdaptType(ETextAdaptType InAdaptType);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMaxLines(float InMaxLines);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void ClearMaxLines();

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMinFontSize(int32 InMinFontSize);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMarqueeDirection(ETextMarqueeDirection InMarqueeDirection);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMarqueePolicy(ETextMarqueePolicy InMarqueePolicy);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMarqueeSpeed(int32 InMarqueeSpeed);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetTextMarqueeType(ETextMarqueeType InTextMarqueeType);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetTextMarqueeStartDelay(float InMarqueeStartDelay);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetTextMarqueeStartDelayOnlyFirst(bool InMarqueeStartDelayOnlyFirst);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		int32 GetCurrentFontSize();

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

	/** When specified, ignore the content's desired size and report the WidthOverride as the Box's desired width. */
	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetWidthOverride(float InWidthOverride);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void ClearWidthOverride();

	/** When specified, ignore the content's desired size and report the HeightOverride as the Box's desired height. */
	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetHeightOverride(float InHeightOverride);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void ClearHeightOverride();

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMinDesiredWidth(float InMinDesiredWidth);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void ClearMinDesiredWidth();

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMinDesiredHeight(float InMinDesiredHeight);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void ClearMinDesiredHeight();

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMaxDesiredWidth(float InMaxDesiredWidth);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void ClearMaxDesiredWidth();

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void SetMaxDesiredHeight(float InMaxDesiredHeight);

	UFUNCTION(BlueprintCallable, Category = "Adapter Text")
		void ClearMaxDesiredHeight();

private:
	void HandleOnTextMarqueeEnd();
	void HandleOnTextFontSizeChanged(int32 FontSize);

public:
	UPROPERTY(EditAnywhere, Category = AdapterText)
		ETextAdaptType AdapterType = ETextAdaptType::Ellipsis;

	UPROPERTY(EditAnywhere, Category = AdapterText, meta = (editcondition = "bOverride_MaxLines"))
		float MaxLines;

	UPROPERTY(EditAnywhere, Category = AdapterText)
		int32 MinFontSize = 10;

	UPROPERTY(VisibleAnywhere, Category = AdapterText)
		int32 CurrentFontSize;

	UPROPERTY(EditAnywhere, Category = AdapterText)
		ETextMarqueeDirection MarqueeDirection = ETextMarqueeDirection::RightToLeft;

	UPROPERTY(EditAnywhere, Category = AdapterText)
		ETextMarqueePolicy MarqueePolicy = ETextMarqueePolicy::Always;

	UPROPERTY(EditAnywhere, Category = AdapterText)
		int32 MarqueeSpeed = 50;

	UPROPERTY(EditAnywhere, Category = AdapterText)
		ETextMarqueeType MarqueeType = ETextMarqueeType::Default;

	UPROPERTY(EditAnywhere, Category = AdapterText)
		float MarqueeStartDelay = 2.0f;

	bool MarqueeStartDelayOnlyFirst = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AdapterText)
		TEnumAsByte<EHorizontalAlignment> HorizontalAlignment = HAlign_Fill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AdapterText)
		TEnumAsByte<EVerticalAlignment> VerticalAlignment = VAlign_Fill;

	/** When specified, ignore the content's desired size and report the WidthOverride as the Box's desired width. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = AdapterText, meta = (editcondition = "bOverride_WidthOverride"))
		float WidthOverride;

	/** When specified, ignore the content's desired size and report the HeightOverride as the Box's desired height. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = AdapterText, meta = (editcondition = "bOverride_HeightOverride"))
		float HeightOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = AdapterText, meta = (editcondition = "bOverride_MinDesiredWidth"))
		float MinDesiredWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = AdapterText, meta = (editcondition = "bOverride_MinDesiredHeight"))
		float MinDesiredHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = AdapterText, meta = (editcondition = "bOverride_MaxDesiredWidth"))
		float MaxDesiredWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = AdapterText, meta = (editcondition = "bOverride_MaxDesiredHeight"))
		float MaxDesiredHeight;

	/**  */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = AdapterText, meta = (InlineEditConditionToggle))
		uint8 bOverride_WidthOverride : 1;

	/**  */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = AdapterText, meta = (InlineEditConditionToggle))
		uint8 bOverride_HeightOverride : 1;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = AdapterText, meta = (InlineEditConditionToggle))
		uint8 bOverride_MinDesiredWidth : 1;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = AdapterText, meta = (InlineEditConditionToggle))
		uint8 bOverride_MinDesiredHeight : 1;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = AdapterText, meta = (InlineEditConditionToggle))
		uint8 bOverride_MaxDesiredWidth : 1;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = AdapterText, meta = (InlineEditConditionToggle))
		uint8 bOverride_MaxDesiredHeight : 1;

	UPROPERTY(EditAnywhere, Category = AdapterText, meta = (InlineEditConditionToggle))
		uint8 bOverride_MaxLines : 1;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "AdapterText Event", meta = (DisplayName = "OnTextMarqueeEnd (Adapter Text)"))
		FOnTextMarqueeEndEvent OnTextMarqueeEnd;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "AdapterText Event", meta = (DisplayName = "OnTextFontSizeChanged (Adapter Text)"))
		FOnTextFontSizeChangedEvent OnTextFontSizeChanged;

protected:
	/**
	 * If this is enabled, text shaping, wrapping, justification are disabled in favor of much faster text layout and measurement.
	 * This feature is only suitable for "simple" text (ie, text containing only numbers or basic ASCII) as it disables the complex text rendering support required for certain languages (such as Arabic and Thai).
	 * It is significantly faster for text that can take advantage of it (particularly if that text changes frequently), but shouldn't be used for localized user-facing text.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Performance, AdvancedDisplay, meta = (AllowPrivateAccess, DesignerRebuild))
		bool bSimpleTextMode;

	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnBindingChanged(const FName& Property) override;
	//~ End UWidget Interface

	/** Get the text that should be displayed in the internal Slate widget (allows flags to mutate the display text without modifying the persistent designer property data) */
	virtual TAttribute<FText> GetDisplayText();

	EVisibility GetTextWarningImageVisibility() const;

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

protected:

	TSharedPtr<SAdaptText> MyTextBlock;

	PROPERTY_BINDING_IMPLEMENTATION(FText, Text);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ColorAndOpacity);
	PROPERTY_BINDING_IMPLEMENTATION(FLinearColor, ShadowColorAndOpacity);
};
