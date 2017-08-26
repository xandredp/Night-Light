// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class No_Bark_VsTarget : TargetRules
{

    public No_Bark_VsTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Game;
        ExtraModuleNames.Add("No_Bark_Vs");
    }
 //   public No_Bark_VsTarget(TargetInfo Target)
	//{
	//	Type = TargetType.Game;
 //       ExtraModuleNames.Add("No_Bark_Vs");
 //   }

	//
	// TargetRules interface.
	//

	//public override void SetupBinaries(
	//	TargetInfo Target,
	//	ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
	//	ref List<string> OutExtraModuleNames
	//	)
	//{
	//	OutExtraModuleNames.AddRange( new string[] { "No_Bark_Vs" } );
	//}
}
