#include "../include/dependents_checker.hpp"

inline bool CheckModuleName(const string &module_name) {
    return module_name.find("Neural") != string::npos;
}

Dependents GetDependentDlls(const wstring &executable_file_path) {
    Dependents dependent_dlls;

    HANDLE file_handle = CreateFileW(executable_file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                                     OPEN_EXISTING, 0, nullptr);
    if (file_handle != INVALID_HANDLE_VALUE) {
        HANDLE mapping_handler = CreateFileMappingW(file_handle, nullptr, PAGE_READONLY | SEC_IMAGE, 0, 0, nullptr);
        if (mapping_handler != INVALID_HANDLE_VALUE) {
            BYTE *base_address = static_cast<BYTE *>(MapViewOfFile(mapping_handler, FILE_MAP_READ, 0, 0, 0));
            if (base_address != nullptr) {
                IMAGE_DOS_HEADER *dos_header = reinterpret_cast<IMAGE_DOS_HEADER *>(base_address);
                IMAGE_NT_HEADERS *nt_headers = reinterpret_cast<IMAGE_NT_HEADERS *>(base_address +
                                                                                    dos_header->e_lfanew);

                IMAGE_DATA_DIRECTORY *import_dir = &(nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
                IMAGE_IMPORT_DESCRIPTOR *import_desc = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR *>(base_address +
                                                                                                   import_dir->VirtualAddress);

                while (import_desc->Name != 0) {
                    LPCSTR lp_module_name = reinterpret_cast<LPCSTR>(base_address + import_desc->Name);
                    string module_name = string(lp_module_name, lp_module_name + strlen(lp_module_name));

                    if (CheckModuleName(module_name)) {
                        dependent_dlls.push_back(module_name);
                    }

                    import_desc++;
                }

                UnmapViewOfFile(base_address);
            } else {
                throw std::runtime_error("Failed to map view of file");
            }

            CloseHandle(mapping_handler);
        } else {
            throw std::runtime_error("Failed to create file mapping");
        }

        CloseHandle(file_handle);
    } else {
        throw std::runtime_error("Failed to open file");
    }

    return dependent_dlls;
}