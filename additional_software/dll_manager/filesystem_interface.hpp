#ifndef NEURALDLLMANAGER_FILESYSTEM_INTERFACE_HPP
#define NEURALDLLMANAGER_FILESYSTEM_INTERFACE_HPP

#include <filesystem>
#include <regex>
#include <vector>
#include <iostream>

#include "CLI/include/ProgressBar.hpp"

using std::wcout;
using std::smatch;
using std::regex;
using std::wstring;
using std::string;
using std::vector;

namespace fs = std::filesystem;

typedef vector<fs::path> Paths;
typedef vector<string> Dependents;

enum class CLI_IO_OPTIONS{
    CLI_ON,
    CLI_OFF
};

string GetDirectory(const fs::path &path);

bool IsCorrectPath(const string &path);

bool IsDependent(const string &path, const Dependents &dependents);

void CopyDLLs(const fs::path &dest, const Paths &dlls, const Dependents &dependents, CLI_IO_OPTIONS io_param);

Paths GetFiles(const fs::path &path, const string &extension);

#endif //NEURALDLLMANAGER_FILESYSTEM_INTERFACE_HPP
