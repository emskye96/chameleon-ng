#pragma once

#include "VMTHook/VMTHook.h"
#include "SDK/SDK.hpp"

#include "imgui.h"
#include "imgui_impl_dx9.h"

#include "FindPattern.hpp"

extern std::unique_ptr<VMTHook> gameevents_hook;
extern std::unique_ptr<VMTHook> clientdll_hook;
extern std::unique_ptr<VMTHook> d3d9_hook;

#include "Hooks/FrameStageNotify.hpp"
#include "Hooks/FireEventClientSide.hpp"
#include "Hooks/IDirect3DDevice9.hpp"