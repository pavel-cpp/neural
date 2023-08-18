#include <array>
#include <filesystem>
#include <regex>
#include <vector>
#include <iostream>

#include "CLI/include/ProgressBar.h"

namespace fs = std::filesystem;

using std::wcout;
using std::endl;
using std::wstring;
using std::string;
using std::vector;
using std::array;
using std::unique_ptr;

typedef vector<fs::path> PathList;

vector<wstring> GetDependentDlls(const wstring& exeFilePath)
{
    std::vector<std::wstring> requiredDlls;

    HANDLE hFile = CreateFileW(exeFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        HANDLE hMapping = CreateFileMappingW(hFile, nullptr, PAGE_READONLY | SEC_IMAGE, 0, 0, nullptr);
        if (hMapping != INVALID_HANDLE_VALUE)
        {
            BYTE* pBaseAddress = static_cast<BYTE*>(MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0));
            if (pBaseAddress != nullptr)
            {
                IMAGE_DOS_HEADER* pDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(pBaseAddress);
                IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(pBaseAddress + pDosHeader->e_lfanew);

                IMAGE_DATA_DIRECTORY* pImportDir = &(pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
                IMAGE_IMPORT_DESCRIPTOR* pImportDesc = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(pBaseAddress + pImportDir->VirtualAddress);

                while (pImportDesc->Name != 0)
                {
                    LPCSTR lpModuleName = reinterpret_cast<LPCSTR>(pBaseAddress + pImportDesc->Name);
                    std::wstring moduleName = std::wstring(lpModuleName, lpModuleName + strlen(lpModuleName));

                    requiredDlls.push_back(moduleName);

                    pImportDesc++;
                }

                UnmapViewOfFile(pBaseAddress);
            }
            else
            {
                std::cout << "Failed to map view of file" << std::endl;
            }

            CloseHandle(hMapping);
        }
        else
        {
            std::cout << "Failed to create file mapping" << std::endl;
        }

        CloseHandle(hFile);
    }
    else
    {
        std::cout << "Failed to open file" << std::endl;
    }

    return requiredDlls;
}

PathList GetFiles(const fs::path &path, const string &extension) {
    PathList files;
    for (const auto &file: fs::directory_iterator(path)) {
        if (file.path().extension() == extension) {
            files.push_back(file);
        }
    }
    return files;
}

string GetDirectory(const fs::path &path) {
    if (path.has_filename() && !is_directory(path)) {
        return path.parent_path().string();
    }
    return path.string();
}

bool IsCorrectPath(const string &path) {
    string fixed_path = GetDirectory(path);
    fixed_path += '\\';
    std::regex expr(R"([A-Z]:\\((\w+\\)*))");
    std::smatch match;
    std::regex_search(path, match, expr);
    return !match.empty();
}

void CopyDLLs(const fs::path &dest, const PathList &dlls) {
    CLI::ProgressBar progress_bar(0, dlls.size());
    for (const auto &file: dlls) {
        try {
            fs::copy(file, dest.string() + "\\" + file.filename().string(), fs::copy_options::overwrite_existing);
        } catch (...) {
            std::wcout << progress_bar.Next(L'\x2593');
            Sleep(100);
            continue;
        }
        std::wcout << progress_bar.Next();
        Sleep(100);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect arguments!" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    fs::path source_path = GetDirectory(argv[0]);
    PathList files = GetFiles(source_path, ".dll");

    if (!IsCorrectPath(argv[1])) {
        std::cerr << "Incorrect Path!" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        vector<wstring> dependents = GetDependentDlls(converter.from_bytes(argv[1]));

        for (const wstring &str: dependents) {
            wcout << str << endl;
        }
    }

    fs::path destination_path = GetDirectory(argv[1]);
    wcout << "[Experimental] Neural DLL manager started!" << endl;
    wcout << "Copying..." << endl;

    CopyDLLs(destination_path, files);

    wcout << endl;

    system("pause");

}
