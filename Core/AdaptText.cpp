// Fill out your copyright notice in the Description page of Project Settings.


#include "AdaptText.h"
#include "SAdaptText.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SInvalidationPanel.h"
#include "Materials/MaterialInstanceDynamic.h"

#define LOCTEXT_NAMESPACE "UMG"

UAdaptText::UAdaptText(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bIsVariable = false;
	bWrapWithInvalidationPanel = false;
	ShadowOffset = FVector2D(1.0f, 1.0f);
	ColorAndOpacity = FLinearColor::White;
	ShadowColorAndOpacity = FLinearColor::Transparent;
	bAutoWrapText_DEPRECATED = false;

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
		Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
		CurrentFontSize = Font.Size;
	}

#if WITH_EDITORONLY_DATA
	AccessibleBehavior = ESlateAccessibleBehavior::Auto;
	bCanChildrenBeAccessible = false;
#endif
}

void UAdaptText::SetColorAndOpacity(FSlateColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UAdaptText::SetOpacity(float InOpacity)
{
	FLinearColor CurrentColor = ColorAndOpacity.GetSpecifiedColor();
	CurrentColor.A = InOpacity;

	SetColorAndOpacity(FSlateColor(CurrentColor));
}

void UAdaptText::SetShadowColorAndOpacity(FLinearColor InShadowColorAndOpacity)
{
	ShadowColorAndOpacity = InShadowColorAndOpacity;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetShadowColorAndOpacity(InShadowColorAndOpacity);
	}
}

void UAdaptText::SetShadowOffset(FVector2D InShadowOffset)
{
	ShadowOffset = InShadowOffset;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetShadowOffset(InShadowOffset);
	}
}

void UAdaptText::SetFont(FSlateFontInfo InFontInfo)
{
	Font = InFontInfo;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetFont(InFontInfo);
	}
}

void UAdaptText::SetStrikeBrush(FSlateBrush InStrikeBrush)
{
	StrikeBrush = InStrikeBrush;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetStrikeBrush(&InStrikeBrush);
	}
}

void UAdaptText::SetMinDesiredWidth(float InMinDesiredWidth)
{
	MinDesiredWidth = InMinDesiredWidth;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetMinDesiredWidth(InMinDesiredWidth);
	}
}

void UAdaptText::SetAutoWrapText(bool InAutoTextWrap)
{
	AutoWrapText = InAutoTextWrap;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetAutoWrapText(InAutoTextWrap);
	}
}

UMaterialInstanceDynamic* UAdaptText::GetDynamicFontMaterial()
{
	if (Font.FontMaterial)
	{
		UMaterialInterface* Material = CastChecked<UMaterialInterface>(Font.FontMaterial);

		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Font.FontMaterial = DynamicMaterial;

			SetFont(Font);
		}

		return DynamicMaterial;
	}

	return nullptr;
}

UMaterialInstanceDynamic* UAdaptText::GetDynamicOutlineMaterial()
{
	if (Font.OutlineSettings.OutlineMaterial)
	{
		UMaterialInterface* Material = CastChecked<UMaterialInterface>(Font.OutlineSettings.OutlineMaterial);

		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Font.OutlineSettings.OutlineMaterial = DynamicMaterial;

			SetFont(Font);
		}

		return DynamicMaterial;
	}

	return nullptr;
}

FText UAdaptText::GetText() const
{
	if (MyTextBlock.IsValid())
	{
		return MyTextBlock->GetText();
	}

	return Text;
}

void UAdaptText::SetText(FText InText)
{
	Text = InText;
	TextDelegate.Unbind();
	if (MyTextBlock.IsValid())
	{
		TAttribute<FText> TextBinding = GetDisplayText();
		MyTextBlock->SetText(TextBinding);
	}
}

void UAdaptText::SetJustification(ETextJustify::Type InJustification)
{
	Super::SetJustification(InJustification);
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetJustification(InJustification);
	}
}

void UAdaptText::SynchronizeProperties()
{
	//UE_LOG(LogTemp, Warning, TEXT("UAdaptText::SynchronizeProperties"));
	Super::SynchronizeProperties();
	TAttribute<FText> TextBinding = GetDisplayText();
	TAttribute<FSlateColor> ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
	TAttribute<FLinearColor> ShadowColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, ShadowColorAndOpacity);
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetText(TextBinding);
		MyTextBlock->SetFont(Font);
		MyTextBlock->SetStrikeBrush(&StrikeBrush);
		MyTextBlock->SetColorAndOpacity(ColorAndOpacityBinding);
		MyTextBlock->SetShadowOffset(ShadowOffset);
		MyTextBlock->SetShadowColorAndOpacity(ShadowColorAndOpacityBinding);
		MyTextBlock->SetMinDesiredWidth(MinDesiredWidth);
		Super::SynchronizeTextLayoutProperties(*MyTextBlock);
		MyTextBlock->SetTextAdaptType(AdapterType);
		MyTextBlock->SetTextMinFontSize(MinFontSize);
		MyTextBlock->SetTextMarqueeDirection(MarqueeDirection);
		MyTextBlock->SetTextMarqueePolicy(MarqueePolicy);
		MyTextBlock->SetTextMarqueeSpeed(MarqueeSpeed);
		MyTextBlock->SetHAlign(HorizontalAlignment);
		MyTextBlock->SetVAlign(VerticalAlignment);
	}
	if (bOverride_WidthOverride)
	{
		SetWidthOverride(WidthOverride);
	}
	else
	{
		ClearWidthOverride();
	}

	if (bOverride_HeightOverride)
	{
		SetHeightOverride(HeightOverride);
	}
	else
	{
		ClearHeightOverride();
	}
}

void UAdaptText::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyTextBlock.Reset();
}

#if WITH_EDITOR
const FText UAdaptText::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

void UAdaptText::OnCreationFromPalette()
{
	Text = LOCTEXT("TextBlockDefaultValue", "Text Block");
}

bool UAdaptText::CanEditChange(const FProperty* InProperty) const
{
	if (bSimpleTextMode && InProperty)
	{
		static TArray<FName> InvalidPropertiesInSimpleMode =
		{
			GET_MEMBER_NAME_CHECKED(UAdaptText, ShapedTextOptions),
			GET_MEMBER_NAME_CHECKED(UAdaptText, Justification),
			GET_MEMBER_NAME_CHECKED(UAdaptText, WrappingPolicy),
			GET_MEMBER_NAME_CHECKED(UAdaptText, AutoWrapText),
			GET_MEMBER_NAME_CHECKED(UAdaptText, WrapTextAt),
			GET_MEMBER_NAME_CHECKED(UAdaptText, Margin),
			GET_MEMBER_NAME_CHECKED(UAdaptText, LineHeightPercentage),
			GET_MEMBER_NAME_CHECKED(UAdaptText, AutoWrapText),
		};

		return !InvalidPropertiesInSimpleMode.Contains(InProperty->GetFName());
	}

	return Super::CanEditChange(InProperty);
}

FString UAdaptText::GetLabelMetadata() const
{
	const int32 MaxSampleLength = 15;

	FString TextStr = Text.ToString().Replace(TEXT("\n"), TEXT(" "));
	TextStr = TextStr.Len() <= MaxSampleLength ? TextStr : TextStr.Left(MaxSampleLength - 2) + TEXT("..");
	return TEXT(" \"") + TextStr + TEXT("\"");
}

void UAdaptText::HandleTextCommitted(const FText& InText, ETextCommit::Type CommitteType)
{
	//TODO UMG How will this migrate to the template?  Seems to me we need the previews to have access to their templates!
	//TODO UMG How will the user click the editable area?  There is an overlay blocking input so that other widgets don't get them.
	//     Need a way to recognize one particular widget and forward things to them!
}

#endif

void UAdaptText::SetAdaptType(ETextAdaptType InAdaptType)
{
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetTextAdaptType(InAdaptType);
	}
}

void UAdaptText::SetMinFontSize(int32 InMinFontSize)
{
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetTextMinFontSize(InMinFontSize);
	}
}

void UAdaptText::SetMarqueeDirection(ETextMarqueeDirection InMarqueeDirection)
{
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetTextMarqueeDirection(InMarqueeDirection);
	}
}

void UAdaptText::SetMarqueePolicy(ETextMarqueePolicy InMarqueePolicy)
{
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetTextMarqueePolicy(InMarqueePolicy);
	}
}

void UAdaptText::SetMarqueeSpeed(int32 InMarqueeSpeed)
{
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetTextMarqueeSpeed(InMarqueeSpeed);
	}
}

int32 UAdaptText::GetCurrentFontSize()
{
	if (MyTextBlock.IsValid())
	{
		return MyTextBlock->GetCurrentFontSize();
	}
	return 0;
}

void UAdaptText::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetHAlign(InHorizontalAlignment);
	}
}

void UAdaptText::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetVAlign(InVerticalAlignment);
	}
}

void UAdaptText::SetWidthOverride(float InWidthOverride)
{
	bOverride_WidthOverride = true;
	WidthOverride = InWidthOverride;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetWidthOverride(InWidthOverride);
	}
}

void UAdaptText::ClearWidthOverride()
{
	bOverride_WidthOverride = false;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetWidthOverride(FOptionalSize());
	}
}

void UAdaptText::SetHeightOverride(float InHeightOverride)
{
	bOverride_HeightOverride = true;
	HeightOverride = InHeightOverride;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetHeightOverride(InHeightOverride);
	}
}

void UAdaptText::ClearHeightOverride()
{
	bOverride_HeightOverride = false;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetHeightOverride(FOptionalSize());
	}
}

void UAdaptText::HandleOnTextMarqueeEnd()
{
	OnTextMarqueeEnd.Broadcast();
}

void UAdaptText::HandleOnTextFontSizeChanged(int32 FontSize)
{
	CurrentFontSize = FontSize;
	OnTextFontSizeChanged.Broadcast(FontSize);
}

void UAdaptText::PostLoad()
{
	Super::PostLoad();

	if (bAutoWrapText_DEPRECATED)
	{
		AutoWrapText = bAutoWrapText_DEPRECATED;
		bAutoWrapText_DEPRECATED = false;
	}
}

TSharedRef<SWidget> UAdaptText::RebuildWidget()
{
	MyTextBlock =
		SNew(SAdaptText)
		.OnTextFontSizeChanged(FOnTextFontSizeChanged::CreateUObject(this, &UAdaptText::HandleOnTextFontSizeChanged))
		.OnTextMarqueeEnd(FOnTextMarqueeEnd::CreateUObject(this, &UAdaptText::HandleOnTextMarqueeEnd))
		.SimpleTextMode(bSimpleTextMode);
	return MyTextBlock.ToSharedRef();
}

void UAdaptText::OnBindingChanged(const FName& Property)
{
	Super::OnBindingChanged(Property);
	if (MyTextBlock.IsValid())
	{
		static const FName TextProperty(TEXT("TextDelegate"));
		static const FName ColorAndOpacityProperty(TEXT("ColorAndOpacityDelegate"));
		static const FName ShadowColorAndOpacityProperty(TEXT("ShadowColorAndOpacityDelegate"));

		if (Property == TextProperty)
		{
			TAttribute<FText> TextBinding = GetDisplayText();
			MyTextBlock->SetText(TextBinding);
		}
		else if (Property == ColorAndOpacityProperty)
		{
			TAttribute<FSlateColor> ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
			MyTextBlock->SetColorAndOpacity(ColorAndOpacityBinding);
		}
		else if (Property == ShadowColorAndOpacityProperty)
		{
			TAttribute<FLinearColor> ShadowColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, ShadowColorAndOpacity);
			MyTextBlock->SetShadowColorAndOpacity(ShadowColorAndOpacityBinding);
		}
	}
}

TAttribute<FText> UAdaptText::GetDisplayText()
{
	return PROPERTY_BINDING(FText, Text);
}

EVisibility UAdaptText::GetTextWarningImageVisibility() const
{
	return Text.IsCultureInvariant() ? EVisibility::Visible : EVisibility::Collapsed;
}

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UAdaptText::GetAccessibleWidget() const
{
	return MyTextBlock;
}
#endif

#undef LOCTEXT_NAMESPACE