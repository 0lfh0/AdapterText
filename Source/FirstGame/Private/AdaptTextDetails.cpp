// Fill out your copyright notice in the Description page of Project Settings.


#include "AdaptTextDetails.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWindow.h"
#include "Widgets/Images/SImage.h"

#define LOCTEXT_NAMESPACE "ADAPTTEXT"

FAdaptTextDetails::FAdaptTextDetails()
{
}

FAdaptTextDetails::~FAdaptTextDetails()
{
}

TSharedRef<IDetailCustomization> FAdaptTextDetails::MakeInstance()
{
	return MakeShareable(new FAdaptTextDetails());
}

void FAdaptTextDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	AdapterTypeProperty = DetailBuilder.GetProperty("AdapterType");

	MaxLinesProperty = DetailBuilder.GetProperty("MaxLines");
	bOverride_MaxLinesProperty = DetailBuilder.GetProperty("bOverride_MaxLines");

	MinFontSizeProperty = DetailBuilder.GetProperty("MinFontSize");
	CurrentFontSizeProperty = DetailBuilder.GetProperty("CurrentFontSize");
	MarqueeDirectionProperty = DetailBuilder.GetProperty("MarqueeDirection");
	MarqueePolicyProperty = DetailBuilder.GetProperty("MarqueePolicy");
	MarqueeSpeedProperty = DetailBuilder.GetProperty("MarqueeSpeed");
	MarqueeTypeProperty = DetailBuilder.GetProperty("MarqueeType");
	MarqueeStartDelayProperty = DetailBuilder.GetProperty("MarqueeStartDelay");

	HAlignmentProperty = DetailBuilder.GetProperty("HorizontalAlignment");
	VAlignmentProperty = DetailBuilder.GetProperty("VerticalAlignment");
	WidthOverrideProperty = DetailBuilder.GetProperty("WidthOverride");
	HeightOverrideProperty = DetailBuilder.GetProperty("HeightOverride");

	MinDesiredWidthProperty = DetailBuilder.GetProperty("MinDesiredWidth");
	MinDesiredHeightProperty = DetailBuilder.GetProperty("MinDesiredHeight");
	MaxDesiredWidthProperty = DetailBuilder.GetProperty("MaxDesiredWidth");
	MaxDesiredHeightProperty = DetailBuilder.GetProperty("MaxDesiredHeight");

	bOverride_WidthOverrideProperty = DetailBuilder.GetProperty("bOverride_WidthOverride");
	bOverride_HeightOverrideProperty = DetailBuilder.GetProperty("bOverride_HeightOverride");
	bOverride_MinDesiredWidthProperty = DetailBuilder.GetProperty("bOverride_MinDesiredWidth");
	bOverride_MinDesiredHeightProperty = DetailBuilder.GetProperty("bOverride_MinDesiredHeight");
	bOverride_MaxDesiredWidthProperty = DetailBuilder.GetProperty("bOverride_MaxDesiredWidth");
	bOverride_MaxDesiredHeightProperty = DetailBuilder.GetProperty("bOverride_MaxDesiredHeight");

	IDetailCategoryBuilder& AdaptTextCategory = DetailBuilder.EditCategory(FName("Adapter Text"));
	AdaptTextCategory.AddProperty(AdapterTypeProperty);

	AdaptTextCategory.AddProperty(MaxLinesProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetEllipsePropertyVisibility)));
	AdaptTextCategory.AddProperty(bOverride_MaxLinesProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetEllipsePropertyVisibility)));

	AdaptTextCategory.AddProperty(MinFontSizeProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetBestFitPropertyVisibility)));
	AdaptTextCategory.AddProperty(CurrentFontSizeProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetBestFitPropertyVisibility)));

	AdaptTextCategory.AddProperty(MarqueeTypeProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetMarqueePropertyVisibility)));
	AdaptTextCategory.AddProperty(MarqueeDirectionProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetMarqueePropertyVisibility)));
	AdaptTextCategory.AddProperty(MarqueePolicyProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetMarqueePropertyVisibility)));
	AdaptTextCategory.AddProperty(MarqueeSpeedProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetMarqueePropertyVisibility)));
	AdaptTextCategory.AddProperty(MarqueeStartDelayProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP< FAdaptTextDetails, uint8>(this, &FAdaptTextDetails::GetMarqueePropertyVisibility, 1)));


	AdaptTextCategory.AddProperty(HAlignmentProperty);
	AdaptTextCategory.AddProperty(VAlignmentProperty);
	AdaptTextCategory.AddProperty(WidthOverrideProperty);
	AdaptTextCategory.AddProperty(HeightOverrideProperty);
	AdaptTextCategory.AddProperty(MinDesiredWidthProperty);
	AdaptTextCategory.AddProperty(MinDesiredHeightProperty);
	AdaptTextCategory.AddProperty(MaxDesiredWidthProperty);
	AdaptTextCategory.AddProperty(MaxDesiredHeightProperty);
	AdaptTextCategory.AddProperty(bOverride_WidthOverrideProperty);
	AdaptTextCategory.AddProperty(bOverride_HeightOverrideProperty);
	AdaptTextCategory.AddProperty(bOverride_MinDesiredWidthProperty);
	AdaptTextCategory.AddProperty(bOverride_MinDesiredHeightProperty);
	AdaptTextCategory.AddProperty(bOverride_MaxDesiredWidthProperty);
	AdaptTextCategory.AddProperty(bOverride_MaxDesiredHeightProperty);
}

EVisibility FAdaptTextDetails::GetEllipsePropertyVisibility()
{
	uint8 AdapterType;
	FPropertyAccess::Result Result = AdapterTypeProperty->GetValue(AdapterType);
	return (Result == FPropertyAccess::MultipleValues || AdapterType == 0) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FAdaptTextDetails::GetBestFitPropertyVisibility()
{
	uint8 AdapterType;
	FPropertyAccess::Result Result = AdapterTypeProperty->GetValue(AdapterType);
	return (Result == FPropertyAccess::MultipleValues || AdapterType == 1) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FAdaptTextDetails::GetMarqueePropertyVisibility()
{
	uint8 AdapterType;
	FPropertyAccess::Result Result = AdapterTypeProperty->GetValue(AdapterType);
	return (Result == FPropertyAccess::MultipleValues || AdapterType == 2) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FAdaptTextDetails::GetMarqueePropertyVisibility(uint8 InMarqueeType)
{
	uint8 AdapterType;
	FPropertyAccess::Result Result = AdapterTypeProperty->GetValue(AdapterType);
	if (Result == FPropertyAccess::MultipleValues || AdapterType == 2)
	{
		uint8 MarqueeType;
		FPropertyAccess::Result MarqueeTypeResult = MarqueeTypeProperty->GetValue(MarqueeType);
		return (MarqueeTypeResult == FPropertyAccess::MultipleValues || MarqueeType == InMarqueeType) ? EVisibility::Visible : EVisibility::Collapsed;
	}
	return EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE