## Simple Project to highlight an issue I found in the UE5 Preview 1 Foliage API

### Why?
I have a use case where I must extract the foliage instances from an Instanced Foliage Actor using a bounding volume.
It worked in **UE5 EA2**

### How!
Load `foliage_extractor.umap` It has some foliage (small red boxes) and an instance of the actor `FoliageExtractor`

`FoliageExtractor` has a `UBoxComponent` whose bounds are encapsulating some of the foliage.
The property "ReferenceFoliage" is set to the same folaige type that I wish to extract (small red boxes)

When the game is ran `FoliageExtractor` will get the IFA(InstancedFoliageActor) for the level and check what instance transforms lie within its `UBoxComponent`.

It will then add them to a child `UHierarchicalInstancedStaticMeshComponent`*` of itself and remove those instances from the the IFA.

If it worked ytou should notice that the extracted meshes turn grey and all lie within the `UBoxComponents` bounds

### Why it doesnt work?
A feature was added to **UE5 Preview 1** for `UHierarchicalInstancedStaticMeshComponent` which affects a call to `IFA->GetOverlappingBoxTransforms`

From what I can gather it modifies the `BuiltInstanceBounds` of the `UHierarchicalInstancedStaticMeshComponent` and can be controlled with `bUseTranslatedInstanceSpace`

It may be that this is expected behaviour but I assume that it is not as `IFA->GetOverlappingBoxTransforms` probably should always return overlapping instances irregardless of the state of `bUseTranslatedInstanceSpace`

### Test it yourself!
Build the project against UE5 Preview 1 and run `foliage_extractor.umap` in the editor. the boxes should all remain red

Uncomment the following and build again to force it to work and you should see the instances within the bounds turn grey

```
//HierarchicalInstancedStaticMeshComp->bUseTranslatedInstanceSpace = false;
//HierarchicalInstancedStaticMeshComp->BuildTreeIfOutdated(false, true);
```