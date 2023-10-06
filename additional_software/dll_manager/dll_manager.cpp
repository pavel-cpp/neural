#include "include/dependents_checker.hpp"
#include "include/filesystem_interface.hpp"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Incorrect arguments!");
        system("pause");
        exit(EXIT_FAILURE);
    }

    fs::path source_path = GetDirectory(argv[0]);
    Paths files = GetFiles(source_path, ".dll");

    if (!IsCorrectPath(argv[1])) {
        throw std::runtime_error("Incorrect Path!");
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
