#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

std::vector<fs::path> FindAllDLL(const fs::path& path){

}

bool IsCorrectPath(const std::string& path){

}

bool CopyDLLs(const std::string& target, const std::vector<fs::path>& form){

}

int main(int argc, char* argv[]){

#ifndef WIN32
    std::cerr << "Only for Windows platfom!" << std::ndl;
#endif

}
