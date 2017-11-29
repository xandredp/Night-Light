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

}
