// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Character_Parent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFrameworks/MyGameInstance.h"
#include "GameFrameworks/MyPlayerController.h"
#include "GameFrameworks/MyGameModeBase.h"
#include "Components/MyStatComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HPWidget.h"
#include "UI/InGame.h"
#include "Weapon.h"
#include "Weapon_Gun.h"
#include "Weapon_Sword.h"
#include "Weapon_Bow.h"
#include "Components/TextBlock.h"
#include "ClimbingWall.h"
#include "DrawDebugHelpers.h"
#include "Enemies/Enemy.h"
#include "Items/Item.h"



// Sets default values
ACharacter_Parent::ACharacter_Parent()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->JumpZVelocity = 500.f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(FName("PlayerCharacter"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}


	CurrentWeaponIndex = 0;

	bIsSprinting = false;
	IsDeath = false;
	ZoomedFOV = 65.f;
	ZoomInterpSpeed = 20.f;

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		Level = MyGameInstanceRef->GetNowLevel();
		// �÷��̾��� ���� ������ 2�Դϴ�. 
	}
}

// Called when the game starts or when spawned
void ACharacter_Parent::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = Camera->FieldOfView;

	Sword = GetWorld()->SpawnActor<AWeapon_Sword>(SwordClass);
	if (Sword)
	{
		Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Sword->SetOwner(this);
	}

	Gun = GetWorld()->SpawnActor<AWeapon_Gun>(GunClass);
	if (Gun)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
		Gun->SetOwner(this);
		Gun->SetHidden(true);
	}

	Bow = GetWorld()->SpawnActor<AWeapon_Bow>(BowClass);
	if (Bow)
	{
		Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BowSocket"));
		Bow->SetOwner(this);
		Bow->SetHidden(true);
	}

	CurrentWeapon = Sword; 
	// ù �⺻ �������� ����� ���Դϴ�. 
}

void ACharacter_Parent::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HpBar->InitWidget();
	UHPWidget* HpWidget = Cast<UHPWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(Stat);
		HpWidget->BindMana(Stat);
		// ĳ���� �Ӹ� ���� ü�¹ٸ� Bind ���ݴϴ�.
	}

	if (MyPlayerController)
	{
		UInGame* MyHUD = Cast<UInGame>(MyPlayerController->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->BindHp(Stat);
			MyHUD->BindMana(Stat);
			MyHUD->BindLevel(Stat);
			// ���� �ϴ��� ü�¹ٿ� ������ Bind ���ݴϴ�.
		}
	}

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyClass, Enemies);
	for (AActor* actor : Enemies)
	{
		AEnemy* Enemy = Cast<AEnemy>(actor);
		Enemy->OnBossJump.AddUObject(this, &ACharacter_Parent::CameraShake);
		Enemy->OnEnemyDeath.AddUObject(this, &ACharacter_Parent::IncreaseExp);
	}

	Stat->OnPlayerLevelUp.AddUObject(this, &ACharacter_Parent::LevelUp);
}

// Called every frame
void ACharacter_Parent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentWeaponIndex == EWeapon::Gun || CurrentWeaponIndex == EWeapon::Bow)
	{
		float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
		float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
		Camera->SetFieldOfView(NewFOV);
		// ���Ⱑ ���̳� Ȱ�� ��� ���콺 ��Ŭ���� �ϸ� ���ε˴ϴ�. 
		// �ڿ������� ������ ���� FInterpTo �Լ��� ����մϴ�. 
	}

	if (bIsClimbingUp && bIsOnWall)
	{
		FVector Loc = GetActorLocation();
		SetActorLocation(FVector(Loc.X, Loc.Y, Loc.Z + 1.5f));
	}

	// �� ���� �� �̻� ���� �� ���� �����ؾ��� ��
	if (bIsClimbingComplete)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * 1.5f);
		SetActorLocation(GetActorLocation() + GetActorUpVector() * LandYsize);
	}
}

// Called to bind functionality to input
void ACharacter_Parent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter_Parent::DoubleJump);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ACharacter_Parent::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ACharacter_Parent::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &ACharacter_Parent::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &ACharacter_Parent::Pitch);
	PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &ACharacter_Parent::CameraZoom);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ACharacter_Parent::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ACharacter_Parent::StopSprinting);
	
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &ACharacter_Parent::BeginZoom);
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Released, this, &ACharacter_Parent::EndZoom);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACharacter_Parent::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACharacter_Parent::EndCrouch);

	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ACharacter_Parent::Interact);
	PlayerInputComponent->BindAction(TEXT("SelectWeapon"), EInputEvent::IE_Pressed, this, &ACharacter_Parent::SelectWeapon);
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &ACharacter_Parent::SelectInventory);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ACharacter_Parent::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &ACharacter_Parent::StopFire);

	PlayerInputComponent->BindAction(TEXT("ClimbingUp"), EInputEvent::IE_Pressed, this, &ACharacter_Parent::PressClimbingUp);

}

void ACharacter_Parent::OnDeath_Implementation()
{
	if (IsDeath)
		return;
	IsDeath = true;

	AMyGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
	if (GameMode)
	{
		GameMode->PawnKilled(this);
		// PawnKilled �Լ��� �ڽ��� �����մϴ�. ���� �й��Դϴ�. 
	}

	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacter_Parent::UpDown(float Value)
{
	if (Value == 0.f || bIsOnWall)
		return;

	AddMovementInput(GetActorForwardVector(), Value);
}

void ACharacter_Parent::LeftRight(float Value)
{
	if (Value == 0.f || bIsOnWall)
		return;

	AddMovementInput(GetActorRightVector(), Value);
}
void ACharacter_Parent::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void ACharacter_Parent::Pitch(float Value)
{
	AddControllerPitchInput(Value);
}

// ShiftŰ�� ���� ĳ������ �̵��ӵ��� ������Ű�� �Լ�
void ACharacter_Parent::Sprint()
{
	if (!bIsSprinting)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	}
}
// ShiftŰ�� �� ĳ������ �̵��ӵ��� ó�� ������ �����ϴ� �Լ�
void ACharacter_Parent::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void ACharacter_Parent::DoubleJump()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bIsOnWall = false;
	// ���� �پ��ִ� ���¿��� ������ �ϸ� ���߿��� �������鼭 Walking ���� ��ȯ�˴ϴ�. 

	if (DoubleJumpCounter == 0)
	{
		ACharacter::LaunchCharacter(FVector(0.f, 0.f, 600.f), false, true);
		IsDoubleJumping = false;
		DoubleJumpCounter++;
	}

	else if (DoubleJumpCounter == 1)
	{
		ACharacter::LaunchCharacter(FVector(0.f, 0.f, 600.f), false, true);
		DoubleJumpCounter++;
		IsDoubleJumping = true;
	}
	else
		return;
}

void ACharacter_Parent::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
}

void ACharacter_Parent::CameraZoom(const float Value)
{
	if (Value == 0.f || !MyPlayerController)
		return;

	const float NewTargetArmLength = SpringArm->TargetArmLength + Value * 10.f;
	SpringArm->TargetArmLength = FMath::Clamp(NewTargetArmLength, 200.f, 1000.f);
	// ���콺 ���� �̿��� 200.f ~ 1000.f ������ �Ÿ��� ������ �� �ֽ��ϴ�. 
}

void ACharacter_Parent::BeginZoom()
{
	bWantsToZoom = true;
}

void ACharacter_Parent::EndZoom()
{
	bWantsToZoom = false;
}

void ACharacter_Parent::BeginCrouch()
{
	Crouch();
}

void ACharacter_Parent::EndCrouch()
{
	UnCrouch();
}

void ACharacter_Parent::Attack()
{
	if (IsAttacking)
		return;

	if (CurrentWeaponIndex == EWeapon::Sword) { Sword->Attack(Stat->GetAttack(), 150.f, nullptr); }
	if (CurrentWeaponIndex == EWeapon::Gun) { Gun->StartFire(); }
	if (CurrentWeaponIndex == EWeapon::Bow) { Bow->Attack(); }
	return;
}

void ACharacter_Parent::StopFire()
{
	if (CurrentWeaponIndex == EWeapon::Gun)
	{
		Gun->StopFire();
		// ���� ���콺�� ���� ���� ������ ����ϴ�. 
	}
}

void ACharacter_Parent::EndAttack_Q()
{
	// Q��ų�� ��Ÿ�� ���� 
	UInGame* InGame = Cast<UInGame>(MyPlayerController->CurrentWidget);

	FString Q_Str;
	if (Remaining_SkillQ < 1) // ��Ÿ���� ������
	{
		Q_Str = FString::Printf(TEXT("Q")); // �ٽ� 'Q' �ؽ�Ʈ�� �ٲ��
		IsAttackingQ = false; // Q ��ų�� ����� �� �ֽ��ϴ�. 
		GetWorldTimerManager().ClearTimer(QSkillHandle); // Ÿ�̸� ����
	}
	else
	{
		Q_Str = FString::Printf(TEXT("%01d "), Remaining_SkillQ); 
		// �����ִ� ��Ÿ���� �����ݴϴ�.
	}

	InGame->Text_Q->SetText(FText::FromString(Q_Str));
	--Remaining_SkillQ; // 1�ʾ� �����մϴ�. 
}

void ACharacter_Parent::EndAttack_E()
{
	UInGame* InGame = Cast<UInGame>(MyPlayerController->CurrentWidget);

	FString E_Str;
	if (Remaining_SkillE < 1)
	{
		E_Str = FString::Printf(TEXT("E"));
		IsAttackingE = false;
		GetWorldTimerManager().ClearTimer(ESkillHandle);
	}
	else
	{
		E_Str = FString::Printf(TEXT("%01d "), Remaining_SkillE);
	}

	InGame->Text_E->SetText(FText::FromString(E_Str));
	--Remaining_SkillE;
}

void ACharacter_Parent::EndAttack_R()
{
	UInGame* InGame = Cast<UInGame>(MyPlayerController->CurrentWidget);
	FString R_Str;

	if (Remaining_SkillR < 1)
	{
		R_Str = FString::Printf(TEXT("R"));
		IsAttackingR = false;
		GetWorldTimerManager().ClearTimer(RSkillHandle);
	}

	else
	{
		R_Str = FString::Printf(TEXT("%01d "), Remaining_SkillR);
	}
	InGame->Text_R->SetText(FText::FromString(R_Str));
	--Remaining_SkillR;
}



float ACharacter_Parent::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->OnAttacked(DamageAmount); // ü���� ���ҵǰ�, ü�¹��� PrgressBar�� ������Ʈ �˴ϴ�. 
	return DamageAmount;
}

// �κ��丮 ����

void ACharacter_Parent::Interact()
{
	// 'Z"Ű�� ���� �������� ������ �� �ֽ��ϴ�. 
	if (CurrentInteractable != nullptr) 
	{
		CurrentInteractable->OnPickedUp(this); 
		// ���� Overlap�� �������� �����Ѵٸ� ������ �� �ֽ��ϴ�. 
	}
}

bool ACharacter_Parent::AddItemToInventory(class AItem* Item)
{
	if (Item != NULL)
	{
		Inventory->AddItem(Item); // �κ��丮�� �������� �߰��մϴ�. 
		CurrentInteractable = nullptr;
		return true;
	}
	return false;
}

void ACharacter_Parent::CheckForInteractable(class AItem* PickItem)
{
	if (PickItem == NULL)
	{
		CurrentInteractable = nullptr;
		return;
	}
	CurrentInteractable = PickItem;
	// ���� Overlap�� �������� �����Ѵٸ� CurrentInteractable�� �־��ݴϴ�. 
}

void ACharacter_Parent::UseItem(class AItem* Item)
{
	Item->Use(this); // Use ���� �Լ��� ȣ���Ͽ� �� �������� ������ ����� �մϴ�. 
	Inventory->RemoveItem(Item); // �κ��丮���� �������� �����մϴ�. 
}

void ACharacter_Parent::SelectWeapon()
{
	// 'O'Ű�� ������ ���� â�� �����ݴϴ�. 
	if (MyPlayerController->bOnWeaponHUD) // �̹� ���� â�� ȭ�鿡 �����Ѵٸ� 
	{
		MyPlayerController->RemoveHUD(MyPlayerController->HS_Weapon);
		// ���� â �������ݴϴ�.
	}
	else
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Weapon);
		// ���� â�� �������� �ʾҴٸ� �߰����ݴϴ�. 
	}
}

void ACharacter_Parent::SelectInventory()
{
	// '��'Ű�� ������ �κ��丮 â�� �����ݴϴ�. 
	if (MyPlayerController->bOnInventoryHUD) // �̹� �κ��丮 â�� ȭ�鿡 �����Ѵٸ� 
	{
		MyPlayerController->RemoveHUD(MyPlayerController->HS_Inventory);
		// �κ��丮 â�� �������ݴϴ�. 
	}
	else
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Inventory);
		// �κ��丮 â�� �������� �ʾҴٸ� �߰����ݴϴ�. 
	}

}

void ACharacter_Parent::SwitchWeapon(int32 Index)
{
	CurrentWeapon->SetActorHiddenInGame(true);	// ���� ���⸦ �����
	CurrentWeaponIndex = Index;					// ���� �ε����� �ٲ��ݴϴ�. 
	SpringArm->TargetArmLength = 450.f;

	if (CurrentWeaponIndex == EWeapon::Sword)
	{
		CurrentWeapon = Sword; 
		SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		SpringArm->SocketOffset = FVector(0.f, 0.f, 0.f);
	}
	else if (CurrentWeaponIndex == EWeapon::Gun)
	{
		CurrentWeapon = Gun;
		SpringArm->SetRelativeLocation(FVector(0.f, 40.f, 70.f));
		SpringArm->SocketOffset = FVector(0.f, 60.f, 0.f);
	}
	else if (CurrentWeaponIndex == EWeapon::Bow)
	{
		CurrentWeapon = Bow;
		SpringArm->SetRelativeLocation(FVector(0.f, 40.f, 70.f));
		SpringArm->SocketOffset = FVector(0.f, 60.f, 0.f);
	}

	Camera->SetFieldOfView(DefaultFOV); // ī�޶�� �þ߸� ó�� ������ �����մϴ�. 
	CurrentWeapon->SetActorHiddenInGame(false); // ���⸦ �����ݴϴ�. 
}

void ACharacter_Parent::PressClimbingUp()
{
	if (bIsClimbingUp)
		return;

	FVector Start = GetCapsuleComponent()->GetRelativeLocation(); // ���� ĸ���ݸ����� ��ġ
	FVector End = GetCapsuleComponent()->GetRelativeLocation() + GetCapsuleComponent()->GetForwardVector() * 300.f; // ���� ��ġ���� 300.f ��

	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float CapsuleHeight = CapsuleHalfHeight * 2.f; // ĸ���� ���̸� ���մϴ�. 

	float diff = 150.f; 
	Start.Z += CapsuleHeight + diff;
	End.Z += CapsuleHeight + diff; 
	// �������� ���� ��� Z������ diff ��ŭ �����ݴϴ�. 
	// ĳ������ ĸ���ݸ��� ���̰� ĳ������ �Ӹ� ������ �������� �ʾ����Ƿ� z������ diff��ŭ �����־����ϴ�.

	FHitResult OutHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	bool bResult = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_GameTraceChannel6, QueryParams);
	// Wall Ʈ���̽� ä���� �����Ͽ����ϴ�. 
	// Wall Ʈ���̽� ä�ο� Block������ �Ͼ�� ���� ����� �� �ֽ��ϴ�. 

	if (bResult) // �浹 ������ �߻��ߴٸ� 
	{
		bIsOnWall = true; // ���� �Ŵ޸� ������ bIsOnWall�� true�� �����մϴ�.
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying); // ĳ���Ͱ� ���� ���ؾ� �ϹǷ� MOVE_Flying ���� �ٲ��ݴϴ�.
		bIsClimbingUp = true; // ����� �� �ֵ��� �ϱ����� bIsClimbingUp�� true�� �����մϴ�. 
	}

	else // �浹 ������ �߻����� �ʾ��� ��
	{
		if (bIsOnWall) // ���� �Ŵ޷��ִ� ���¶��
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �ٽ� MOVE_Walking ���� �ٲٰ�
			bIsOnWall = false; // �� �̻� ���� �Ŵ޸� ���°� �ƴϰ�
			bIsClimbingComplete = true; // ����� �������� �˸��� bIsClimbingComplete�� true�� �����մϴ�. 
		}
		else { return; } 
	}

	GetWorld()->GetTimerManager().SetTimer(ClimbingHandle, this, &ACharacter_Parent::ReleaseClimbingUp, 1.5f, true);
	// ��� �ִϸ��̼��� �Ϸ�Ǳ���� ��ٸ��� ���� 1.5�� �� ReleaseClimbingUp �Լ��� ȣ���մϴ�. 
}

void ACharacter_Parent::ReleaseClimbingUp()
{
	bIsClimbingUp = false; // �� �� ����� �Ϸ�� �� bIsClimbingUp�� false�� �˴ϴ�. 
	GetWorld()->GetTimerManager().ClearTimer(ClimbingHandle); 
	bIsClimbingComplete = false;
}

void ACharacter_Parent::CameraShakeCheck()
{
	if (AttackCamShake)
	{
		CameraShake(AttackCamShake);
	}
}

void ACharacter_Parent::CameraShake(TSubclassOf<class UCameraShakeBase> Cam)
{
	if (MyPlayerController)
	{
		MyPlayerController->ClientPlayCameraShake(Cam);
		// �÷��̾� ��Ʈ�ѷ��� �̿��� ī�޶� �����ݴϴ�. 
	}
}

void ACharacter_Parent::LevelUp()
{
	// ��ƼŬ ����
	UGameplayStatics::SpawnEmitterAttached(LevelupEffect, GetMesh());
	UGameplayStatics::PlaySoundAtLocation(this, LevelupSound, GetActorLocation());

	// �������Ͽ� �÷��̾��� ü���� Max������ �����մϴ�.
	Level++;
	MyGameInstanceRef->SetNowLevel(Level);
	Stat->SetPlayerLevel(Level);
}

UMyStatComponent* ACharacter_Parent::GetMyStatComponent()
{
	if (Stat)
	{
		return Stat;
	}
	return nullptr;
}

UInventoryComponent* ACharacter_Parent::GetInventoryComponent()
{
	if (Inventory)
	{
		return Inventory;
	}
	return nullptr;
}

void ACharacter_Parent::IncreaseExp(int32 Exp)
{
	Stat->SetExp(Exp);
}


