#pragma once

inline void**& GetVirtualTable(void* baseclass) {
	return *static_cast<void***>(baseclass);
}

template <typename Fn = void*> inline Fn GetVirtualFunction(void* baseclass, size_t index) {
	return reinterpret_cast<Fn>(GetVirtualTable(baseclass)[index]);
}