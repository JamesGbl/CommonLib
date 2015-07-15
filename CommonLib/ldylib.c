#include "ldylib.h"

#ifdef _WIN32
#include <windows.h>
dylib_handle_t ldylib_load(const char *libname, lerror **error)
{
    dylib_handle_t result = LoadLibraryA(libname);
    if (result==NULL) {
        lerror_set_sprintf(error, "Cannot open dynamic library \"%s\", please verify that a file with that name does exist and that add its dependencies are met", libname);
    }

    return result;
}

void ldylib_unload(dylib_handle_t handle)
{
    FreeLibrary(handle);
}

void* ldylib_get_sym(dylib_handle_t handle, const char *sym_name, lerror **error)
{
    void *result = GetProcAddress(handle, sym_name);
    if (result==NULL)
    {
        lerror_set_sprintf(error, "Can't find symbol \"%s\" in the specified library", sym_name);
    }

    return result;
}
#else
#include <dlfcn.h>

dylib_handle_t ldylib_load(const char *libname, lerror **error)
{
    dylib_handle_t result = dlopen(libname, RTLD_LAZY);
    if (result==NULL) {
        lerror_set_sprintf(error, "Cannot open dynamic library \"%s\", please verify that a file with that name does exist and that add its dependencies are met", libname);
    }

    return result;
}

void ldylib_unload(dylib_handle_t handle)
{
    dlclose(handle);
}

void* ldylib_get_sym(dylib_handle_t handle, const char *sym_name, lerror **error)
{
    void *result = dlsym(handle, sym_name);
    if (result==NULL)
    {
        lerror_set_sprintf(error, "Can't find symbol \"%s\" in the specified library", sym_name);
    }

    return result;
}
#endif
