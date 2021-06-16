// Fill out your copyright notice in the Description page of Project Settings.


#include "AdaptTextDetails.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWindow.h"
#include "Widgets/Images/SImage.h"
#include "../SliceEditor/SSliceEditor.h"

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
	MinFontSizeProperty = DetailBuilder.GetProperty("MinFontSize");
	CurrentFontSizeProperty = DetailBuilder.GetProperty("CurrentFontSize");
	MarqueeDirectionProperty = DetailBuilder.GetProperty("MarqueeDirection");
	MarqueePolicyProperty = DetailBuilder.GetProperty("MarqueePolicy");
	MarqueeSpeedProperty = DetailBuilder.GetProperty("MarqueeSpeed");
	HAlignmentProperty = DetailBuilder.GetProperty("HorizontalAlignment");
	VAlignmentProperty = DetailBuilder.GetProperty("VerticalAlignment");
	WidthOverrideProperty = DetailBuilder.GetProperty("WidthOverride");
	HeightOverrideProperty = DetailBuilder.GetProperty("HeightOverride");
	bOverride_WidthOverrideProperty = DetailBuilder.GetProperty("bOverride_WidthOverride");
	bOverride_HeightOverrideProperty = DetailBuilder.GetProperty("bOverride_HeightOverride");

	IDetailCategoryBuilder& AdaptTextCategory = DetailBuilder.EditCategory(FName("Adapter Text"));
	AdaptTextCategory.AddProperty(AdapterTypeProperty);
	AdaptTextCategory.AddProperty(MinFontSizeProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetBestFitPropertyVisibility)));
	AdaptTextCategory.AddProperty(CurrentFontSizeProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetBestFitPropertyVisibility)));
	AdaptTextCategory.AddProperty(MarqueeDirectionProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetMarqueePropertyVisibility)));
	AdaptTextCategory.AddProperty(MarqueePolicyProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetMarqueePropertyVisibility)));
	AdaptTextCategory.AddProperty(MarqueeSpeedProperty)
		.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAdaptTextDetails::GetMarqueePropertyVisibility)));
	AdaptTextCategory.AddProperty(HAlignmentProperty);
	AdaptTextCategory.AddProperty(VAlignmentProperty);
	AdaptTextCategory.AddProperty(WidthOverrideProperty);
	AdaptTextCategory.AddProperty(HeightOverrideProperty);
	AdaptTextCategory.AddProperty(bOverride_WidthOverrideProperty);
	AdaptTextCategory.AddProperty(bOverride_HeightOverrideProperty);

	//================================================================
	TSharedPtr<IPropertyHandle> TestBrushProperty = DetailBuilder.GetProperty("TestBrush");
	AdaptTextCategory.AddProperty(TestBrushProperty);
	AdaptTextCategory.AddCustomRow(FText::GetEmpty()).WholeRowContent()
		[
			SNew(SButton)
			.OnClicked_Lambda([this, TestBrushProperty]() { return OpenWindow(TestBrushProperty); })
			[
				SNew(STextBlock).Text(FText::FromString("hello world"))
			]
		];
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

FReply FAdaptTextDetails::OpenWindow(TSharedPtr< IPropertyHandle> InTestBrushProperty)
{
	FSliceEditorArgs EditorArgs;
	if (InTestBrushProperty.IsValid() && InTestBrushProperty->GetProperty())
	{
		TArray<void*> RowData;
		InTestBrushProperty->AccessRawData(RowData);
		if (RowData.Num() > 0)
		{
			FSlateBrush* SlateBrush = static_cast<FSlateBrush*>(RowData[0]);
			if (SlateBrush)
			{
				EditorArgs.Image = SlateBrush;
			}
		}
	}
	OpenSliceEditor(EditorArgs);
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE