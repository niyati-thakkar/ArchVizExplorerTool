// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArchVizUserWidget.generated.h"

/**
 * 
 */

class AArchVizPlayerController;

UCLASS()
class EXPLORERTOOL_API UArchVizUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	AArchVizPlayerController* PlayerController;
};
