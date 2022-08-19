﻿#pragma once
#include "IMergeActorsTool.h"
#include "GigaMergeTool.generated.h"

class UGigaMesh;
class SGigaMergingDialog;

UCLASS(Config=Engine)
class UGigaMergeToolSettings : public UObject
{
	GENERATED_BODY()
public:
	UGigaMergeToolSettings()
	{
		Settings.bMergePhysicsData = true;
		Settings.LODSelectionType = EMeshLODSelectionType::AllLODs;
	}

	static UGigaMergeToolSettings* Get()
	{
		if (!DefaultSettings)
		{
			DefaultSettings = DuplicateObject(GetMutableDefault<UGigaMergeToolSettings>(), nullptr);
			DefaultSettings->AddToRoot();
		}
		return DefaultSettings;
	}

	static void Destroy()
	{
		if (UObjectInitialized() && DefaultSettings)
		{
			DefaultSettings->RemoveFromRoot();
			DefaultSettings->MarkPendingKill();
		}
		DefaultSettings = nullptr;
	}
	
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties), Category=MergeSettings)
	FMeshMergingSettings Settings;
	
private:
	static UGigaMergeToolSettings* DefaultSettings;
};

class FGigaMergeTool : public IMergeActorsTool
{
public:
	FGigaMergeTool();
	virtual ~FGigaMergeTool() override;

	// Begin IMergeActorsTool Implementation
	virtual FName GetIconName() const override { return "MergeActors.MeshMergingTool"; }
	virtual FText GetTooltipText() const override;
	virtual TSharedRef<SWidget> GetWidget() override;
	virtual FString GetDefaultPackageName() const override;
	virtual bool CanMerge() const override;
	virtual bool RunMerge(const FString& PackageName) override;
	// End IMergeActorsTool

	virtual FString GetDefaultAssetPackageName(FString PackageName = FString{}) const;
	
private:
	TSharedPtr<SGigaMergingDialog> MergingDialog;
	UGigaMergeToolSettings* Settings;

	void MergeComponents(const FString& PackageName, const TArray<UPrimitiveComponent*>& Components, TArray<UObject*>& OutAssets, FVector& OutPivot) const;
	UGigaMesh* DuplicateGigaMesh(FString&& AssetName, UObject* Asset) const;
};
