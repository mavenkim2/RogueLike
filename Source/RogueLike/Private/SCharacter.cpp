// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "SProjectileBaseClass.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("Interaction Component");

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("Attribute Component");
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Vector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	AddMovementInput(YawRotation.Vector(), Vector.Y);
	FVector RightVector = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Vector.X);
}

void ASCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Vector= Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(Vector.X);
		AddControllerPitchInput(Vector.Y);
	}
}

void ASCharacter::Jump(const FInputActionValue& Value)
{
	if (GetController() && Value.Get<bool>())
	{
		Super::Jump();
	}
}


void ASCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimation);
	FTimerDelegate AttackDelegate; 
	AttackDelegate.BindUFunction(this, "ProjectileTimeElapsed", AttackProjectile);
	GetWorldTimerManager().SetTimer(TimerHandleProjectile, AttackDelegate, 0.2f, false);
}

void ASCharacter::Teleport()
{
	PlayAnimMontage(AttackAnimation);
	FTimerDelegate TeleportDelegate;
	TeleportDelegate.BindUFunction(this, "ProjectileTimeElapsed", TeleportProjectile);
	//FTimerDelegate TeleportDelegate = FTimerDelegate::CreateUObject(this, &ASCharacter::ProjectileTimeElapsed, TeleportProjectile);
	GetWorldTimerManager().SetTimer(TimerHandleProjectile, TeleportDelegate, 0.2f, false);
}

void ASCharacter::Blackhole()
{
	PlayAnimMontage(AttackAnimation);
	FTimerDelegate BlackholeDelegate; 
	BlackholeDelegate.BindUFunction(this, "ProjectileTimeElapsed", BlackholeProjectile);
	GetWorldTimerManager().SetTimer(TimerHandleProjectile, BlackholeDelegate, 0.2f, false);
}

void ASCharacter::ProjectileTimeElapsed(TSubclassOf<ASProjectileBaseClass> ProjectileClass)
{
	if (ensureAlways(ProjectileClass))
	{
		GetWorldTimerManager().ClearTimer(TimerHandleProjectile);
		FVector SpawnLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = this;

		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		QueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
	
		FHitResult HitResult;
		FVector StartLocation = CameraComponent->GetComponentLocation();
		FVector EndLocation = StartLocation + (GetControlRotation().Vector() * 5000);

		if (GetWorld()->SweepSingleByObjectType(HitResult, StartLocation, EndLocation, FQuat::Identity, QueryParams, FCollisionShape::MakeSphere(20.f), Params))
		{
			EndLocation = HitResult.ImpactPoint;
		}
	
		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(EndLocation - SpawnLocation).Rotator();
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, ProjectileRotation, SpawnParameters);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
		EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
		EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Triggered, this, &ASCharacter::Teleport);
		EnhancedInputComponent->BindAction(BlackholeAction, ETriggerEvent::Triggered, this, &ASCharacter::Blackhole);
	}
}

