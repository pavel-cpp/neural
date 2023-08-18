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

// [Experimental] Function to execute a shell command and return the result
string ExecuteShellCommand(const string& command)
{
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

// [Experimental] Function to get a list of dependent modules for a given EXE file
vector<string> GetDependentDlls(const string& exePath)
{
    vector<string> dependentDlls;

    string command = "dumpbin /dependents \"" + exePath + "\"";
    string output = ExecuteShellCommand(command);

    size_t startPos = output.find("Image has the following dependencies:");
    if (startPos != string::npos)
    {
        output = output.substr(startPos + strlen("Image has the following dependencies:"));

        size_t endPos = output.find("\n");
        if (endPos != string::npos)
        {
            output = output.substr(0, endPos);

            size_t pos = 0;
            string delimiter = "\n";
            while ((pos = output.find(delimiter)) != string::npos)
            {
                string dllName = output.substr(0, pos);
                dependentDlls.push_back(dllName);
                output.erase(0, pos + delimiter.length());
            }
        }
    }

    return dependentDlls;
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

    fs::path destination_path = GetDirectory(argv[1]);
    wcout << "[Experimental] Neural DLL manager started!" << endl;
    wcout << "Copying..." << endl;

    CopyDLLs(destination_path, files);

    wcout << endl;

    system("pause");

}
