// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class No_Bark_Vs : ModuleRules
{
    public No_Bark_Vs(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "GameplayTasks",
            "MoviePlayer"
            //AI
           // "AIModule"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            //To Use UMG in C++
            "Slate",
            "SlateCore"
        });
    }


 //   public No_Bark_Vs(TargetInfo Target)
	//{
	//	PublicDependencyModuleNames.AddRange(new string[] {
 //           "Core",
 //           "CoreUObject",
 //           "Engine",
 //           "InputCore",

 //           "UMG",
 //           "GameplayTasks",

 //       });

	//	PrivateDependencyModuleNames.AddRange(new string[] {  });

	//	// Uncomment if you are using Slate UI
	//	 PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
	//	// Uncomment if you are using online features
	//	// PrivateDependencyModuleNames.Add("OnlineSubsystem");

	//	// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	//}
}
