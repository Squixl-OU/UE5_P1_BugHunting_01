// Fill out your copyright notice in the Description page of Project Settings.


#include "FoliageExtractor.h"

#include "Components/BoxComponent.h"
#include "Foliage/Public/FoliageType.h"
#include "Foliage/Public/InstancedFoliageActor.h"


// Sets default values
AFoliageExtractor::AFoliageExtractor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExtractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Extraction Volume"));
	RootComponent = ExtractionVolume;
	

}

// Called when the game starts or when spawned
void AFoliageExtractor::BeginPlay()
{
	Super::BeginPlay();

	ExtractFoliage();
}

// Called every frame
void AFoliageExtractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFoliageExtractor::ExtractFoliage()
{
	if(!IsValid(ReferenceFoliage)){UE_LOG(LogTemp, Warning, TEXT("ReferenceFoliage is not Valid")); return;}

	// clear the instance transforms array
	InstanceTransforms.Empty();

	// Grab the IFA and populate Instance Transforms Array with overlapping transforms
	IFA = AInstancedFoliageActor::GetInstancedFoliageActorForLevel(AActor::GetWorld()->GetLevel(0), /*bCreateIfNone*/ false); // TODO: change this to account for more than 1 IFA
	const FBox tempBox = ExtractionVolume->CalcBounds(ExtractionVolume->GetComponentTransform()).GetBox();

	// Get the UInstancedStaticMeshComponent component from the IFA
	HierarchicalInstancedStaticMeshComp = Cast<UHierarchicalInstancedStaticMeshComponent>(IFA->GetComponentByClass(UHierarchicalInstancedStaticMeshComponent::StaticClass()));
	
	// These 2 lines need to be uncommented for "GetOverlappingBoxTransforms" to work again
	//HierarchicalInstancedStaticMeshComp->bUseTranslatedInstanceSpace = false;
	//HierarchicalInstancedStaticMeshComp->BuildTreeIfOutdated(false, true);

	UE_LOG(LogTemp, Warning, TEXT("CalcBounds Result: Min: %s, Max: %s, Center: %s"), *tempBox.Min.ToCompactString(), *tempBox.Max.ToCompactString(), *tempBox.GetCenter().ToCompactString());
	IFA->GetOverlappingBoxTransforms(ReferenceFoliage, tempBox, InstanceTransforms);

	for (auto transform : InstanceTransforms)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping Transform: %s"), *transform.GetTranslation().ToCompactString())
	}

	// Remove all instances from the IFA that are stored in InstanceTransforms
	for (int i = HierarchicalInstancedStaticMeshComp->GetInstanceCount() - 1; i >= 0; i--)
	{
		FTransform Out;
		HierarchicalInstancedStaticMeshComp->GetInstanceTransform(i, Out);

		for (auto transform : InstanceTransforms)
		{
			if(transform.Equals(Out))
			{
				HierarchicalInstancedStaticMeshComp->RemoveInstance(i);
			}
		}
	}

	FoliageChunk = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, UHierarchicalInstancedStaticMeshComponent::StaticClass(), NAME_None);

	// check if valid, register with world & addInstanceComp to actor
	if(!IsValid(FoliageChunk)){UE_LOG(LogTemp, Warning, TEXT("FieldChunk is not valid")); return;}
	FoliageChunk->RegisterComponentWithWorld(GetWorld());
	this->AddInstanceComponent(FoliageChunk);

	// check if cast is valid, set static mesh instance & add all instances to HISM using InstanceTransforms
	if(!IsValid(Cast<UFoliageType_InstancedStaticMesh>(ReferenceFoliage)->Mesh)){UE_LOG(LogTemp, Warning, TEXT("SetStaticMesh Cast failed")); return;}
	FoliageChunk->SetStaticMesh(Cast<UFoliageType_InstancedStaticMesh>(ReferenceFoliage)->Mesh);

	FoliageChunk->AddInstances(InstanceTransforms, false);
}

