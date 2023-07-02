// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionProfileName("PhysicsActor");

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("Radial Force");
	RadialForce->SetupAttachment(GetRootComponent());
	RadialForce->Radius = 1000.f;
	RadialForce->bImpulseVelChange = true;
	
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::Explosion);
}

void ASExplosiveBarrel::Explosion(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForce->FireImpulse();
}
