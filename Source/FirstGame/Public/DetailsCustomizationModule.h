// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class FIRSTGAME_API FDetailsCustomizationModule : public FDefaultGameModuleImpl
{
public:
	FDetailsCustomizationModule();
	~FDetailsCustomizationModule();

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
