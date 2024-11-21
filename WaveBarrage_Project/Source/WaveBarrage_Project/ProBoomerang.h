// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBProjectileBase.h"
#include "ProBoomerang.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AProBoomerang : public AWBProjectileBase
{
	GENERATED_BODY()
	
public:
	AProBoomerang();

protected:

	virtual void BeginPlay() override;

	void MoveForward(FVector Direction);

	void MoveBackWard(FVector Direction);

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	float Time =0.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector AttackDirection;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ProjectileSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration = 0.0f;

	bool CanCollision = false;

	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
