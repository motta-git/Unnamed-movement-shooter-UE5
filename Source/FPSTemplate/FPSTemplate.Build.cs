// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class FPSTemplate : ModuleRules
{
	public FPSTemplate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Si usas GameplayStatics ya está en Engine.
	}
}
