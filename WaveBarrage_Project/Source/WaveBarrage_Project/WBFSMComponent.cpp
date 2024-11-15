// Fill out your copyright notice in the Description page of Project Settings.


#include "WBFSMComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	if (StateTime > 0.1f)
	{
		ChangeState(EMonsterState::Move);
	}
}

void UWBFSMComponent::MoveState(float DeltaTime)
{
	//Ÿ�� �÷��̾� �������� �̵�
	FVector Dist = GetOwner()->GetActorForwardVector() * MoveDistance * DeltaTime;
	GetOwner()->AddActorWorldOffset(Dist);

	//�ƴϸ鼭 ���� �ð��� ������ IdleState
	if (CanAttack)
	{
		//�̵� �� CanAttack �� true �̰� �Ÿ��� ��Ÿ� ���̶�� AttackState
		ChangeState(EMonsterState::Attack);
	}
	else
	{
		if (StateTime > 1.0f)
		{
			ChangeState(EMonsterState::Idle);
		}
	}
}

void UWBFSMComponent::AttackState()
{
	//���Ÿ� , ���� ���� ����
	//����ü �߻� �� ���� �ð� ������ IdleState
	if (StateTime > 1.0f)
	{
		ChangeState(EMonsterState::Idle);
	}
}

void UWBFSMComponent::SkillState()
{
	// ���� ����
	// ���� ���� ���� �ݸ��� �˻��ؼ� ���� ���� �÷��̾�� ������
	// ���� �ð� ������ IdleState
	if (StateTime > 1.0f)
	{
		ChangeState(EMonsterState::Idle);
	}
}

void UWBFSMComponent::DieState()
{
	// Destroy Actor �����ϴ� ���� �������� �ʵ��� �ϱ� ����
}


void UWBFSMComponent::StateTick(float DeltaTime)
{
	StateTime += DeltaTime;
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
	case EMonsterState::Die:
		DieState();
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

