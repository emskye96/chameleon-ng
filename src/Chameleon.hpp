#pragma once

#include "VMTHook/VMTHook.h"

#include "SDK/SDK.hpp"

extern std::unique_ptr<VMTHook> gameevents_hook;
extern std::unique_ptr<VMTHook> clientdll_hook;

#include "Hooks/FrameStageNotify.hpp"
#include "Hooks/FireEventClientSide.hpp"