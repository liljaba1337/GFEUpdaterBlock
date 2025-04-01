#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <filesystem>
#include <json.hpp>

using json = nlohmann::json;

std::string GetRegistryValue(const std::string& keyPath, const std::string& valueName) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        std::cerr << "Error opening registry key." << std::endl;
        return "";
    }

    char value[512];
    DWORD valueLength = sizeof(value);
    if (RegQueryValueExA(hKey, valueName.c_str(), NULL, NULL, (LPBYTE)value, &valueLength) != ERROR_SUCCESS) {
        std::cerr << "Error reading registry value." << std::endl;
        RegCloseKey(hKey);
        return "";
    }

    RegCloseKey(hKey);
    return std::string(value, valueLength);
}

void ModifyJsonFile(const std::string& jsonFilePath) {
    std::ifstream inputFile(jsonFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening JSON file." << std::endl;
        return;
    }
    std::string backupPath = std::filesystem::path(jsonFilePath).replace_extension(".json.backup").string();
    if (!std::filesystem::exists(backupPath)) {
        std::filesystem::copy(jsonFilePath, backupPath);
    }
    else {
        std::cerr << "Backup file already exists. Skipping." << std::endl;
    }

    json jsonData;
    inputFile >> jsonData;
    inputFile.close();

    auto& switches = jsonData["switches"];
    bool patched = false;
    for (auto it = switches.begin(); it != switches.end(); ++it) {
        if (it->get<std::string>().find("nv-self-update-path") != std::string::npos) {
            switches.erase(it);
            patched = true;
            break;
        }
    }
    if (patched) {
        std::ofstream outputFile(jsonFilePath);
        if (!outputFile.is_open()) {
            std::cerr << "Error opening JSON file for writing." << std::endl;
            return;
        }

        outputFile << jsonData.dump(4);
        outputFile.close();
        std::cout << "Successfully modified the JSON file." << std::endl;
    }
    else {
        std::cout << "Couldn't find the required line in the json file. Most likely, it has already been patched. If not, please open an issue." << std::endl;
    }
}

int main() {
    std::string regKey = R"(SOFTWARE\NVIDIA Corporation\Global\GFExperience)";
    std::string regValue = "FullPath";

    std::string path = GetRegistryValue(regKey, regValue);
    if (path.empty()) {
        std::cout << "Couldn't get the path automatically. Enter your NVIDIA GeForce Experience.json path (Usually C:\\Program Files\\NVIDIA Corporation\\NVIDIA GeForce Experience\\NVIDIA GeForce Experience.json): ";
        std::cin >> path;
    }
    size_t pos = path.rfind(".exe");
    if (pos != std::string::npos) {
        path.replace(pos, 4, ".json");
    }
    else {
        std::cerr << "Invalid registry value. This error is extremely unlikely to happen, so I don't handle it properly. If you face it, please open an issue." << std::endl;
        return 1;
    }

    ModifyJsonFile(path);

    system("pause");
    return 0;
}
