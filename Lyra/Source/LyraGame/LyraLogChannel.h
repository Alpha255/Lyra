// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Logging/LogMacros.h"

LYRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLyra, Log, All);

LYRAGAME_API FString GetClientServerContextString(class UObject* ContextObject = nullptr);
