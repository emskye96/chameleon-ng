#pragma once

inline void**& GetVirtualTable(void* baseclass) {
	return *static_cast<void***>(baseclass);
}

template <typename Fn> inline Fn GetVirtualFunction(void* vftable, size_t index) {
	return reinterpret_cast<Fn>(GetVirtualTable(vftable)[index]);
}