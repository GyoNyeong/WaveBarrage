// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWBPlayerST.generated.h"

USTRUCT(BlueprintType)
struct FWBPlayerST
{
public:
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlayerID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HP;

	FWBPlayerST()
		: PlayerID(0), HP(1000) {}

	// 파라미터 생성자
	FWBPlayerST(int32 InPlayerID, int32 InHP)
		: PlayerID(InPlayerID), HP(InHP) {}
};

