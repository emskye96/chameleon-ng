#pragma once

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

// Helper function to generate a random sequence number.
inline const int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}

// Replacement function that will be called when the view model animation sequence changes.
void __cdecl hkSequenceProxyFn(const CRecvProxyData* proxy_data_const, void* entity, void* output) {
	// Get the original proxy function used by the game and store it in a static variable for later usage.
	static RecvVarProxyFn oSequenceProxyFn = sequence_hook->GetOriginalFunction();

	// Get a pointer to our local player entity
	C_BasePlayer* localplayer = static_cast<C_BasePlayer*>(entitylist->GetClientEntity(engine->GetLocalPlayer()));

	// Don't go any further if we're not alive.
	if (!localplayer || localplayer->GetLifeState() != LIFE_ALIVE)
		return oSequenceProxyFn(proxy_data_const, entity, output);

	// Remove the constness from the proxy data allowing us to make changes.
	CRecvProxyData* proxy_data = const_cast<CRecvProxyData*>(proxy_data_const);

	// Cast the view model entity to the correct class.
	C_BaseViewModel* viewmodel = static_cast<C_BaseViewModel*>(entity);

	if (viewmodel && viewmodel->GetOwner()) {
		// Ensure the owner handle is valid.
		if (viewmodel->GetOwner() == INVALID_EHANDLE_INDEX)
			return oSequenceProxyFn(proxy_data_const, entity, output);

		// Convert the view model owner handle to a player pointer.
		C_BasePlayer* owner = static_cast<C_BasePlayer*>(entitylist->GetClientEntityFromHandle(viewmodel->GetOwner()));
		
		// Compare the owner entity of this view model to the local player entity.
		if (owner == entitylist->GetClientEntity(engine->GetLocalPlayer())) {
			// Get the filename of the current view model.
			const model_t* knife_model = modelinfo->GetModel(*viewmodel->GetModelIndex());
			const char* model_filename = modelinfo->GetModelName(knife_model);
			
			// Set the current sequence.
			int m_nSequence = proxy_data->m_Value.m_Int;

			// Check the model filename to determine which fixes to use.
			if (!strcmp(model_filename, "models/weapons/v_knife_butterfly.mdl")) {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
					case SEQUENCE_DEFAULT_DRAW:
						m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
					default:
						m_nSequence++;
				}
			} else if (!strcmp(model_filename, "models/weapons/v_knife_falchion_advanced.mdl")) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
					case SEQUENCE_DEFAULT_HEAVY_MISS1:
						m_nSequence = RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					default:
						m_nSequence--;
				}
			} else if (!strcmp(model_filename, "models/weapons/v_knife_push.mdl")) {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
					case SEQUENCE_DEFAULT_LIGHT_MISS1:
					case SEQUENCE_DEFAULT_LIGHT_MISS2:
						m_nSequence = RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
					case SEQUENCE_DEFAULT_HEAVY_MISS1:
						m_nSequence = RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
					case SEQUENCE_DEFAULT_HEAVY_HIT1:
					case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence += 3; break;
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					default:
						m_nSequence += 2;
				}
			} else if (!strcmp(model_filename, "models/weapons/v_knife_survival_bowie.mdl")) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
					default:
						m_nSequence--;
				}
			}

			// Set the fixed sequence.
			proxy_data->m_Value.m_Int = m_nSequence;
		}
	}

	// Call the original function with our edited data.
	oSequenceProxyFn(proxy_data, entity, output);
}