#include "ClassRegistry.h"
#include "exceptions/InternalErrorException.h"

#include <cstdlib>
#include <cstdio>
#include <bfd.h>
#include <dlfcn.h>
#include <error.h>
#include <vector>
#include <string>

std::atomic<int> ClassRegistry::m_counter = 0;

void ClassRegistry::Initialize() {
    asymbol *minisyms;
    long symcount;
    unsigned int size;
    char **matching;

    bfd *file = bfd_openr("testsyms.so", nullptr);
    if (file == nullptr) {
        std::string error = 
            std::string("bfd_openr() failed. Error message: ") + 
            std::string(bfd_errmsg(bfd_get_error()));
        throw InternalErrorException(error.c_str());
    }

    if (!bfd_check_format_matches(file, bfd_object, &matching)) {
        std::string error =
            std::string("bfd_check_format_matches() failed. Error message: ") +
            std::string(bfd_errmsg(bfd_get_error()));
        throw InternalErrorException(error.c_str());
    }

    if (!(bfd_get_file_flags(file) & HAS_SYMS))
    {
        std::string error =
            std::string("bfd_get_file_flags() failed. Error message: ") +
            std::string(bfd_errmsg(bfd_get_error()));
        throw InternalErrorException(error.c_str());
    }

    symcount = bfd_read_minisymbols(file, FALSE, (void**)&minisyms, &size);
    if (symcount < 0) {
        std::string error =
            std::string("bfd_read_minisymbols() failed. Error message: ") +
            std::string(bfd_errmsg(bfd_get_error()));
        throw InternalErrorException(error.c_str());
    }

    asymbol *store = bfd_make_empty_symbol(file);

    bfd_byte *from = (bfd_byte *) minisyms;
    bfd_byte *fromend = from + symcount * size;

    std::vector<std::string> classFactoryNames;

    for (; from < fromend; from += size) {
        asymbol *sym = bfd_minisymbol_to_symbol(file, FALSE, from, store);
        classFactoryNames.push_back(sym->name);
    }

    if (minisyms != nullptr) {
        free(minisyms);
    } 
    if (matching != nullptr) {
        free(matching);
    }

    if (!bfd_close(file)) {
        std::string error =
            std::string("bfd_close() failed. Error message: ") +
            std::string(bfd_errmsg(bfd_get_error()));
        throw InternalErrorException(error.c_str());
    }

    m_libraryHandle = dlopen(NULL, RTLD_NOW | RTLD_LOCAL);
    if (m_libraryHandle == NULL) {
        std::string error =
        std::string("dlopen() failed. Error code: ") +
        std::to_string(errno);
    }


    for (std::string classFactoryName : classFactoryNames) {
        if (classFactoryName.compare(0, std::string("Factory_").length(), "Factory_") == 0) {
            const IClass& (*ClassFactoryFcn)() =
                (const IClass & (*)())dlsym(m_libraryHandle, classFactoryName.c_str());
            m_classes.emplace(classFactoryName.substr(std::string("Factory_").length()), Class(ClassFactoryFcn()));
        }
    }
}

void ClassRegistry::Uninitialize() {
    dlclose(m_libraryHandle);
}
