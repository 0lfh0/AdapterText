// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "SAdaptText.h"

/**
 *
 */
class FIRSTGAME_API FAdaptTextDetails : public IDetailCustomization
{
public:
	FAdaptTextDetails();
	~FAdaptTextDetails();

public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	EVisibility GetEllipsePropertyVisibility();
	EVisibility GetBestFitPropertyVisibility();
	EVisibility GetMarqueePropertyVisibility();
	EVisibility GetMarqueePropertyVisibility(uint8 InMarqueeType);

	TSharedPtr<IPropertyHandle> AdapterTypeProperty;
	TSharedPtr<IPropertyHandle> MaxLinesProperty;
	TSharedPtr<IPropertyHandle> bOverride_MaxLinesProperty;
	TSharedPtr<IPropertyHandle> MinFontSizeProperty;
	TSharedPtr<IPropertyHandle> CurrentFontSizeProperty;
	TSharedPtr<IPropertyHandle> MarqueeDirectionProperty;
	TSharedPtr<IPropertyHandle> MarqueePolicyProperty;
	TSharedPtr<IPropertyHandle> MarqueeSpeedProperty;
	TSharedPtr<IPropertyHandle> MarqueeTypeProperty;
	TSharedPtr<IPropertyHandle> MarqueeStartDelayProperty;

	TSharedPtr<IPropertyHandle> HAlignmentProperty;
	TSharedPtr<IPropertyHandle> VAlignmentProperty;
	TSharedPtr<IPropertyHandle> WidthOverrideProperty;
	TSharedPtr<IPropertyHandle> HeightOverrideProperty;
	TSharedPtr<IPropertyHandle> MinDesiredWidthProperty;
	TSharedPtr<IPropertyHandle> MinDesiredHeightProperty;
	TSharedPtr<IPropertyHandle> MaxDesiredWidthProperty;
	TSharedPtr<IPropertyHandle> MaxDesiredHeightProperty;
	TSharedPtr<IPropertyHandle> bOverride_WidthOverrideProperty;
	TSharedPtr<IPropertyHandle> bOverride_HeightOverrideProperty;
	TSharedPtr<IPropertyHandle> bOverride_MinDesiredWidthProperty;
	TSharedPtr<IPropertyHandle> bOverride_MinDesiredHeightProperty;
	TSharedPtr<IPropertyHandle> bOverride_MaxDesiredWidthProperty;
	TSharedPtr<IPropertyHandle> bOverride_MaxDesiredHeightProperty;
};
