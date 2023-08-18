#include "filesystem_interface.hpp"

namespace fs = std::filesystem;

using std::wcout;
using std::cout;
using std::cerr;
using std::smatch;
using std::regex;
using std::endl;
using std::wstring;
using std::string;
using std::vector;

typedef vector<fs::path> Paths;
typedef vector<string> Dependents;

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
                cout << "Failed to map view of file" << endl;
            }

            CloseHandle(mapping_handler);
        } else {
            cout << "Failed to create file mapping" << endl;
        }

        CloseHandle(file_handle);
    } else {
        cout << "Failed to open file" << endl;
    }

    return dependent_dlls;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Incorrect arguments!" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    fs::path source_path = GetDirectory(argv[0]);
    Paths files = GetFiles(source_path, ".dll");

    if (!IsCorrectPath(argv[1])) {
        cerr << "Incorrect Path!" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    fs::path destination_path = GetDirectory(argv[1]);
    wcout << L"[Experimental] Neural DLL manager started!" << endl;
    wcout << L"Checking dependents..." << endl;

    Dependents dependents;
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        dependents = GetDependentDlls(converter.from_bytes(argv[1]));

        for (const string &str: dependents) {
            cout << str << endl;
        }
    }

    wcout << L"Copying..." << endl;

    CopyDLLs(destination_path, files, dependents);

    wcout << endl;

    system("pause");

}
