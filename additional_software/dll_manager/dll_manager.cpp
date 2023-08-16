#include <filesystem>
#include <regex>
#include <vector>
#include <iostream>

#include "CLI/include/ProgressBar.h"

namespace fs = std::filesystem;

typedef std::vector<fs::path> PathList;

PathList GetFiles(const fs::path &path, const std::string &extension) {
    PathList files;
    for (const auto &file: fs::directory_iterator(path)) {
        if (file.path().extension() == extension) {
            files.push_back(file);
        }
    }
    return files;
}

std::string GetDirectory(const fs::path &path) {
    if (path.has_filename() && !is_directory(path)) {
        return path.parent_path().string();
    }
    return path.string();
}

void deleteFileName(fs::path &path) {
    if (path.has_filename() && !is_directory(path)) {
        path = path.parent_path();
    }
}

bool IsCorrectPath(const std::string &path) {
    std::string fixed_path = GetDirectory(path);
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
        std::cerr << "Incorrect arguments!" << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < argc; ++i) {
        std::cout << i << " -> " << argv[i] << std::endl;
    }

    fs::path source_path = GetDirectory(argv[0]);
    PathList files = GetFiles(source_path, ".dll");

    if (!IsCorrectPath(argv[1])) {
        std::cerr << "Incorrect Path!" << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    fs::path destination_path = GetDirectory(argv[1]);
    std::wcout << "Neural DLL manager started!" << std::endl;
    std::wcout << "Copying..." << std::endl;

    CopyDLLs(destination_path, files);

    std::wcout << std::endl;

    system("pause");

}
