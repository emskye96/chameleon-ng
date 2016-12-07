#pragma once

// Signature of 'CreateInterface', see tier1/interface.h in the Source SDK 2013 code.
typedef void* (*CreateInterface_t) (const char*, int*);

template <class T> inline T* CaptureInterface(const char* filename, const char* version) {
	// Get a handle to the requested DLL file.
	HMODULE module_handle = GetModuleHandleA(filename);
	
	// Get the address of the "CreateInterface" function in the specified DLL.
	FARPROC create_interface_proc = GetProcAddress(module_handle, "CreateInterface");

	// Cast the function to the above signature so we can call it.
	CreateInterface_t create_interface_fn = reinterpret_cast<CreateInterface_t>(create_interface_proc);

	// Call the function and return a pointer to the returned interface.
	return static_cast<T*>(create_interface_fn(version, nullptr));
}