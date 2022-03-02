// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FoliageExtractor.generated.h"

class UFoliageType;
class UBoxComponent;

UCLASS()
class UE5_BUGHUNTING_01_API AFoliageExtractor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFoliageExtractor();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Foliage Extractor")
	UBoxComponent* ExtractionVolume;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Foliage Extractor")
	AInstancedFoliageActor* IFA;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Foliage Extractor")
	UFoliageType* ReferenceFoliage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Foliage Extractor")
	TArray< FTransform > InstanceTransforms;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Foliage Extractor")
	UHierarchicalInstancedStaticMeshComponent* FoliageChunk;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Foliage Extractor")
	UHierarchicalInstancedStaticMeshComponent* HierarchicalInstancedStaticMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Exec)
	void ExtractFoliage();
};
