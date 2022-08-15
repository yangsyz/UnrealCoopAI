// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealCoopAITarget : TargetRules
{
	public UnrealCoopAITarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bUseUnityBuild = false;
		ExtraModuleNames.AddRange( new string[] { "UnrealCoopAI" } );
	}
}
