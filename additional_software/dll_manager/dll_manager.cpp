#include <filesystem>
#include <iostream>
#include <vector>
#include <regex>

namespace fs = std::filesystem;

typedef std::vector<fs::path> PathList;

PathList FindAllDLL(const fs::path& path){
    PathList files;
    for(const auto& file : fs::directory_iterator(path)){
        if(file.is_regular_file()){
            files.push_back(file);
        }
    }
    return files;
}

std::string deleteFileName(const fs::path& path){
    if(path.has_filename()){
        return path.parent_path().string();
    }
    return path.string();
}

void deleteFileName(fs::path& path){
    if(path.has_filename()){
        path = path.parent_path();
    }
}

bool IsCorrectPath(const std::string& path){
    std::string fixed_path = deleteFileName(path);
    fixed_path += '\\';
    std::regex expr(R"([A-Z]:\\((\w+\\)*))");
    std::smatch match;
    std::regex_search(path, match, expr);
    return !match.empty() && match[0].str() == fixed_path;
}

void CopyDLLs(const fs::path& dest, const PathList& dlls){
    for(const auto& file: dlls){
        fs::copy_file(file, dest, fs::copy_options::overwrite_existing);
        std::cout << file.filename() << " copied!";
    }
}

int main(int argc, char* argv[]){
    if(argc > 2){
        std::cerr << "Too many arguments!" << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    fs::path source_path = deleteFileName(argv[0]);
    PathList files = FindAllDLL(source_path);

    if(!IsCorrectPath(argv[1])) {
        std::cerr << "Incorrect Path!" << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    fs::path destination_path = deleteFileName(argv[1]);

    std::cout << "Neural DLL loader started!" << std::endl;

    system("pause");
}
