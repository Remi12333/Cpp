#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <vector>
#include <string>
#include <stdexcept>

namespace fs = std::filesystem;

std::vector<std::string> findFiles(const std::string& directory, const std::vector<std::string>& extensions) {
    std::vector<std::string> files;
    try {
        if (!fs::exists(directory)) {
            throw std::runtime_error("Я ничего не нашол ты брехло" + directory);
        }

        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                for (const auto& ext : extensions) {
                    if (entry.path().extension() == ext) {
                        files.push_back(entry.path().string());
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Ошибка переделай " << e.what() << std::endl;
        exit(1);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        exit(1);
    }
    return files;
}

void executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Ошибка! Alarm! Warning! " << command << std::endl;
        exit(1);
    }
}

int main() {
    const std::string githubRepoUrl = "https://github.com/Remi12333/Cpp.git";
    const std::string localDirectory = "C:\\git\\gitload";
    const std::string commitMessage = "Автоматическая загрузка файлов";
    const std::vector<std::string> extensions = {".cpp", ".js", ".html", ".css"};

    std::vector<std::string> files = findFiles(localDirectory, extensions);

    if (files.empty()) {
        std::cout << "Нет файлов для загрузки в указанной директории: " << localDirectory << std::endl;
        return 0;
    }
    executeCommand("git init");
    executeCommand("git remote add origin " + githubRepoUrl);

    for (const auto& file : files) {
        executeCommand("git add \"" + file + "\"");
    }
    executeCommand("git commit -m \"" + commitMessage + "\"");
    executeCommand("git branch -M main");
    executeCommand("git push -u origin main");

    std::cout << "Молодец:)" << std::endl;

    return 0;
}
