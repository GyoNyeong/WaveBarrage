// Fill out your copyright notice in the Description page of Project Settings.


#include "WBFSMComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WBMonsterBase.h"
#include "WBMonsterProjectile.h"

// Sets default values for this component's properties
UWBFSMComponent::UWBFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MoveDistance = 200.0f;
	// ...
}


// Called when the game starts
void UWBFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	MState = EMonsterState::Idle;
	
}

void UWBFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	StateTick(DeltaTime);
}

void UWBFSMComponent::IdleState()
{
	// ���� ����
	if (IsValid(TargetPlayer))
	{
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetPlayer->GetActorLocation());
		GetOwner()->SetActorRotation(TargetRot);
	}

	ChangeState(EMonsterState::Move);
}

void UWBFSMComponent::MoveState(float DeltaTime)
{
	// ���� ĳ��Ʈ �ؼ� �տ� ���ͷ� ���� ������ ���ߵ��� ����
	// 
	//Ÿ�� �÷��̾� �������� �̵�
	FVector Dist = GetOwner()->GetActorForwardVector() * MoveDistance * DeltaTime;
	GetOwner()->AddActorWorldOffset(Dist, true);

	//�ƴϸ鼭 ���� �ð��� ������ IdleState
	if (CanAttack && !bIsAttackDelay)
	{
		//�̵� �� CanAttack �� true �̰� �Ÿ��� ��Ÿ� ���̶�� AttackState
		ChangeState(EMonsterState::Attack);
	}
	else if (CanSkill && !bIsSkillDelay)
	{
		ChangeState(EMonsterState::Skill);
	}
	else
	{
		if (CanMove && StateTime > 1.0f)
		{
			ChangeState(EMonsterState::Idle);
		}
	}
}

void UWBFSMComponent::AttackState()
{
	//���Ÿ� ����
	//����ü �߻� �� ���� �ð� ������ IdleState
	if (!bIsAttackDelay)
	{
		bIsAttackDelay = true;
		Cast<AWBMonsterBase>(GetOwner())->Attack();
	}

	if (StateTime > 0.5f)
	{
		ChangeState(EMonsterState::Idle);
	}
}

void UWBFSMComponent::SkillState()
{
	// ����Ʈ ����
	// ���� ���� ���� �ݸ��� �˻��ؼ� ���� ���� �÷��̾�� ������
	// ���� �ð� ������ IdleState
	if (!bIsSkillDelay)
	{
		bIsSkillDelay = true;
		Cast<AWBMonsterBase>(GetOwner())->Skill();
	}

	if (StateTime > 1.0f)
	{
		ChangeState(EMonsterState::Idle);
	}
}


void UWBFSMComponent::StateTick(float DeltaTime)
{
	StateTime += DeltaTime;
	if (bIsAttackDelay)
	{
		AttackTime += DeltaTime;
		if (AttackTime > 5.0f)
		{
			bIsAttackDelay = false;
			AttackTime = 0;
		}
	}
	if (bIsSkillDelay)
	{
		SkillTime += DeltaTime;
		if (SkillTime > 5.0f)
		{
			bIsSkillDelay = false;
			SkillTime = 0;
		}
	}

	switch (MState)
	{
	case EMonsterState::Idle:
		IdleState();
		break;
	case EMonsterState::Move:
		MoveState(DeltaTime);
		break;
	case EMonsterState::Attack:
		AttackState();
		break;
	case EMonsterState::Skill:
		SkillState();
		break;
	default:
		break;
	}
}

void UWBFSMComponent::ChangeState(EMonsterState State)
{
	MState = State;
	StateTime = 0;
}

