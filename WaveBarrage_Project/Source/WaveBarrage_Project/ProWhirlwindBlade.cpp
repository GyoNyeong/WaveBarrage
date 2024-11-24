// Fill out your copyright notice in the Description page of Project Settings.


#include "ProWhirlwindBlade.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "WBMonsterBase.h"
#include "Net/UnrealNetwork.h"

AProWhirlwindBlade::AProWhirlwindBlade()
{
	bReplicates = true;
	OrbitSpeed = 180.0f;
	OrbitRadius = 360.0f;
	SetReplicateMovement(false);
}

void AProWhirlwindBlade::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{// ���������� Ÿ�̸� ����
		GetWorldTimerManager().SetTimer(OrbitTimerHandle, this, &AProWhirlwindBlade::UpdatePosition, 0.016f, true,0.0f);
	}

}

void AProWhirlwindBlade::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AProWhirlwindBlade::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("WhirlwindOverlap"));

	if (OtherActor && OtherActor != this)
	{
		if (HasAuthority() && CanCollision == true)
		{
			AWBMonsterBase* Monster = Cast<AWBMonsterBase>(OtherActor);
			if (Monster)
			{
				UGameplayStatics::ApplyDamage(Monster, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

			}
		}
	}
}

void AProWhirlwindBlade::UpdatePosition()
{
	if (!HasAuthority())

	return;

	CurAngle += OrbitSpeed * 0.016f; 
	float RadianAngle = UKismetMathLibrary::DegreesToRadians(CurAngle + SpawnLocationByRadians);

	/*ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
	if (MyCharacter)*/
	if(GetOwner())
	{
		Center = GetOwner()->GetActorLocation();

		float X = cos(RadianAngle) * OrbitRadius;
		float Y = sin(RadianAngle) * OrbitRadius;

		FVector NewPosition = Center + FVector(X, Y, 0);

		SetActorLocation(NewPosition); // �������� ��ġ ������Ʈ
		Multicast_UpdatePosition(NewPosition); // Ŭ���̾�Ʈ�鿡�� ��ġ ����

	}
}

void AProWhirlwindBlade::Multicast_UpdatePosition_Implementation(FVector NewPosition)
{
	if (HasAuthority())
	{
		return;
	}
	else
	{
		SetActorLocation(NewPosition);
	}
}

void AProWhirlwindBlade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProWhirlwindBlade, SpawnLocationByRadians);
	DOREPLIFETIME(AProWhirlwindBlade, OrbitRadius);
	DOREPLIFETIME(AProWhirlwindBlade, OrbitSpeed);
	DOREPLIFETIME(AProWhirlwindBlade, CanCollision);
}
