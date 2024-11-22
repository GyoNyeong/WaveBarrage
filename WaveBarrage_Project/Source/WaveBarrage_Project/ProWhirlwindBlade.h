// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBProjectileBase.h"
#include "ProWhirlwindBlade.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AProWhirlwindBlade : public AWBProjectileBase
{
	GENERATED_BODY()

public:
	AProWhirlwindBlade();
	
protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    // �������� ��ġ ��� �� ������Ʈ
    void UpdatePosition();

    // Ŭ���̾�Ʈ�� ��ġ ����ȭ
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_UpdatePosition(FVector NewPosition);
    void Multicast_UpdatePosition_Implementation(FVector NewPosition);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    FTimerHandle OrbitTimerHandle;

    // ���� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CurAngle;

    // �߽� ��ġ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FVector Center;





    // ȸ�� �ӵ� (��/��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    float OrbitSpeed;

    // ȸ�� �ݰ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    float OrbitRadius;

    // ���� ��ġ�� ����
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Replicated)
    float SpawnLocationByRadians;

    // �浹 ���� ����
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Replicated)
    bool CanCollision;

 
};
