// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailsCustomizationModule.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "AdaptTextDetails.h"

FDetailsCustomizationModule::FDetailsCustomizationModule()
{
}

FDetailsCustomizationModule::~FDetailsCustomizationModule()
{
}

void FDetailsCustomizationModule::StartupModule()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout("AdaptText", FOnGetDetailCustomizationInstance::CreateStatic(&FAdaptTextDetails::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FDetailsCustomizationModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomClassLayout("AdaptText");
}
