// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TaskBarDataAsset.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ButtonCategory : uint8
{
	Roads,
	Infrastructure,
	Interior,
	Materials,
	SaveLoad
};
USTRUCT(BlueprintType)
struct FTaskBarButtonData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ButtonImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ButtonCategory ButtonText;

};

UCLASS()
class EXPLORERTOOL_API UTaskBarDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskBarButtonData> ButtonData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Logo;
};
