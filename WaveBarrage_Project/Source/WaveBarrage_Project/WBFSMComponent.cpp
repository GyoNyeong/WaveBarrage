// Fill out your copyright notice in the Description page of Project Settings.


#include "WBFSMComponent.h"

// Sets default values for this component's properties
UWBFSMComponent::UWBFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

	switch (MState)
	{
	case EMonsterState::Idle:
		IdleState(DeltaTime);
		break;
	case EMonsterState::Move:
		MoveState(DeltaTime);
		break;
	case EMonsterState::Attack:
		AttackState(DeltaTime);
		break;
	case EMonsterState::Skill:
		SkillState(DeltaTime);
		break;
	case EMonsterState::Die:
		DieState(DeltaTime);
		break;
	default:
		break;
	}
}

void UWBFSMComponent::IdleState(float DeltaTime)
{
	// ��� ���
	StateTime += DeltaTime;

	if (StateTime > 0.25f)
	{
		MState = EMonsterState::Move;
		StateTime = 0;
	}
}

void UWBFSMComponent::MoveState(float DeltaTime)
{
	StateTime += DeltaTime;
	//Ÿ�� �÷��̾� �������� �̵�
	
	//�ƴϸ鼭 ���� �ð��� ������ IdleState
	if (CanAttack)
	{
		//�̵� �� CanAttack �� true �̰� �Ÿ��� ��Ÿ� ���̶�� AttackState

	}
	else
	{
		if (StateTime > 1.0f)
		{
			MState = EMonsterState::Idle;
			StateTime = 0;
		}
	}
}

void UWBFSMComponent::AttackState(float DeltaTime)
{
	//���Ÿ� , ���� ���� ����
	//����ü �߻� �� ���� �ð� ������ IdleState
	if (StateTime > 1.0f)
	{
		MState = EMonsterState::Idle;
		StateTime = 0;
	}
}

void UWBFSMComponent::SkillState(float DeltaTime)
{
	// ���� ����
	// ���� ���� ���� �ݸ��� �˻��ؼ� ���� ���� �÷��̾�� ������
	// ���� �ð� ������ IdleState
	if (StateTime > 1.0f)
	{
		MState = EMonsterState::Idle;
		StateTime = 0;
	}
}

void UWBFSMComponent::DieState(float DeltaTime)
{
	// Destroy Actor �����ϴ� ���� �������� �ʵ��� �ϱ� ����
}


// Called every frame


void UWBFSMComponent::ChangeState()
{
}

