#include "filesystem_interface.hpp"

string GetDirectory(const fs::path &path) {
    if (path.has_filename() && !is_directory(path)) {
        return path.parent_path().string();
    }
    return path.string();
}

bool IsCorrectPath(const string &path) {
    regex expr(R"([A-Z]:\\((\w+\\)*))");
    smatch match;
    regex_search(path, match, expr);
    return !match.empty();
}

bool IsDependent(const string &path, const Dependents &dependents) {
    return std::any_of(dependents.begin(), dependents.end(), [path](const string &dependent) {
        return path.find(dependent) != string::npos;
    });
}

void CopyDLLs(const fs::path &dest, const Paths &dlls, const Dependents &dependents, CLI_IO_OPTIONS io_param = CLI_IO_OPTIONS::CLI_ON) {
    CLI::ProgressBar progress_bar(0, static_cast<int>(dlls.size()));
    for (const auto &file: dlls) {
        if (!IsDependent(file.string(), dependents)) continue;
        try {
            fs::copy(file, dest.string() + "\\" + file.filename().string(), fs::copy_options::overwrite_existing);
        } catch (...) {
            if(io_param == CLI_IO_OPTIONS::CLI_ON) {
                wcout << progress_bar.Next(L'\x2593');
                Sleep(100);
            }
            continue;
        }
        if(io_param == CLI_IO_OPTIONS::CLI_ON) {
            wcout << progress_bar.Next();
            Sleep(100);
        }
    }
}

Paths GetFiles(const fs::path &path, const string &extension) {
    Paths files;
    for (const auto &file: fs::directory_iterator(path)) {
        if (file.path().extension() == extension) {
            files.push_back(file);
        }
    }
    return files;
}