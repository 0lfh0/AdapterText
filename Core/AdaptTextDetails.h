// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "SAdaptText.h"

/**
 * 
 */
class TESTGAME_API FAdaptTextDetails : public IDetailCustomization
{
public:
	FAdaptTextDetails();
	~FAdaptTextDetails();

public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	EVisibility GetBestFitPropertyVisibility();
	EVisibility GetMarqueePropertyVisibility();

	TSharedPtr<IPropertyHandle> AdapterTypeProperty;
	TSharedPtr<IPropertyHandle> MinFontSizeProperty;
	TSharedPtr<IPropertyHandle> CurrentFontSizeProperty;
	TSharedPtr<IPropertyHandle> MarqueeDirectionProperty;
	TSharedPtr<IPropertyHandle> MarqueePolicyProperty;
	TSharedPtr<IPropertyHandle> MarqueeSpeedProperty;
	TSharedPtr<IPropertyHandle> HAlignmentProperty;
	TSharedPtr<IPropertyHandle> VAlignmentProperty;
	TSharedPtr<IPropertyHandle> WidthOverrideProperty;
	TSharedPtr<IPropertyHandle> HeightOverrideProperty;
	TSharedPtr<IPropertyHandle> bOverride_WidthOverrideProperty;
	TSharedPtr<IPropertyHandle> bOverride_HeightOverrideProperty;

	FReply OpenWindow(TSharedPtr< IPropertyHandle> InTestBrushProperty);
};
