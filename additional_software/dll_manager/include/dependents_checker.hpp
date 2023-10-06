#ifndef NEURALDLLMANAGER_DEPENDENTS_CHECKER_HPP
#define NEURALDLLMANAGER_DEPENDENTS_CHECKER_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <windows.h>

using std::wstring;
using std::string;
using std::vector;

typedef vector<string> Dependents;

inline bool CheckModuleName(const string &module_name);

Dependents GetDependentDlls(const wstring &executable_file_path);

#endif //NEURALDLLMANAGER_DEPENDENTS_CHECKER_HPP
