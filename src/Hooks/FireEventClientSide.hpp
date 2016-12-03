#pragma once

// Function signature of IGameEventManager2::FireEventClientSide.
typedef bool (__thiscall *FireEventClientSide_t) (IGameEventManager2*, IGameEvent*);

// Replacement function that will intercept game events before they are fired.
bool __fastcall hkFireEventClientSide(IGameEventManager2* thisptr, void* edx, IGameEvent* event) {
	// Get the original function and store it in a static variable for later usage.
	static FireEventClientSide_t oFireEventClientSide = gameevents_hook->GetOriginalFunction<FireEventClientSide_t>(9);

	// Filter to only the events we're interested in.
	if (!strcmp(event->GetName(), "player_death")) {
		// Check if we were the attacker in this event.
		int attacker_id = event->GetInt("attacker");
		
		if (engine->GetPlayerForUserID(attacker_id) == engine->GetLocalPlayer()) {
			// Check if we have an override for the weapon we used.
			const char* weapon = event->GetString("weapon");

			// Override the weapon icon that will be displayed in the kill feed.
			if (!strcmp(weapon, "knife_default_ct")) {
				event->SetString("weapon", "knife_karambit");
			} else if (!strcmp(weapon, "knife_t")) {
				event->SetString("weapon", "knife_m9_bayonet");
			}
		}
	}

	// Pass the event to the original function.
	return oFireEventClientSide(thisptr, event);
}