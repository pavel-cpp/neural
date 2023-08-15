#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

typedef std::vector<fs::path> PathList;

PathList FindAllDLL(const fs::path& path){
    PathList files;
    for(const auto& file : fs::directory_iterator(path)){
        files.push_back(file);
    }
    return files;
}

bool IsCorrectPath(const std::string& path){

}

bool CopyDLLs(const fs::path& target, const std::vector<fs::path>& form){

}

int main(int argc, char* argv[]){

#ifndef WIN32
    std::cerr << "Only for Windows platfom!" << std::ndl;
#endif

    for(int i = 0; i < argc; ++i){
        std::cout << i << argv[i] << std::endl;
        fs::path path = argv[i];
        if(path.has_filename()){
            path = path.parent_path();
        }
        PathList files = FindAllDLL(path);
    }

}
