#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <algorithm>

#include "http_reader.h"
#include "main.h"

void print_supported_releases(const nlohmann::json_abi_v3_11_3::json &data, std::vector<std::string>& supported_versions)
{
    std::cout << "supported ubuntu releases\n";
    supported_versions.clear();
    for (auto product : data["products"])
    {
        if(product["arch"] != "amd64")
            continue;
        if(product["supported"])
            supported_versions.push_back(product["version"]);
    }
}

int compareVersions(const std::string& v1, const std::string& v2) {
    std::stringstream ss1(v1), ss2(v2);
    int part1, part2;
    while (ss1 || ss2) {
        if (!(ss1 >> part1)) part1 = 0; // Default to 0 if no more parts in v1
        if (!(ss2 >> part2)) part2 = 0; // Default to 0 if no more parts in v2
        if (part1 != part2) return part1 < part2 ? -1 : 1; // Compare parts
        ss1.ignore(1, '.'); // Ignore the dot separator
        ss2.ignore(1, '.');
    }
    return 0; // Versions are equal
}

std::string print_latest_lts_release(const nlohmann::json_abi_v3_11_3::json &data)
{
    //TODO: check only LTS versions
    auto products_list = data["products"];
    // Find the highest version

     auto max_version = std::max_element(
        products_list.begin(), products_list.end(),
        [](const auto& a, const auto& b) {
            return compareVersions(a["version"], b["version"]) < 0; // Compare by version number
        }
    );

    return (*max_version)["version"];
}

std::string sha_of_disk_1(const nlohmann::json_abi_v3_11_3::json &data, const std::string& version_name)
{
    const std::string specific_version = "20160420.3";
    return data["products"][version_name]["versions"][specific_version]["items"]["disk1.img"]["sha256"];
}


int main() {
    std::string url = "https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json";
    std::string outputPath = "download.json"; // TODO: download to resources folder
    
    // if (downloadFile(url, outputPath)) {
    //     std::cout << "File downloaded successfully to " << outputPath << std::endl;
    // } else {
    //     std::cout << "File download failed" << std::endl;
    // }

    std::ifstream jasonFile(outputPath);
    nlohmann::json data = nlohmann::json::parse(jasonFile);
    std::vector<std::string> supported_versions = {};
    // std::cout << "Parsed JSON Data:\n";
    // std::cout << data.dump(4) << std::endl;

    print_supported_releases(data, supported_versions);
    for(const auto& version : supported_versions)
        std::cout << version << std::endl;

    std::cout << "latest ubuntu release " << print_latest_lts_release(data) << std::endl;

    std::string ubuntu_release_name = "com.ubuntu.cloud:server:16.04:arm64";
    std::cout << sha_of_disk_1(data, ubuntu_release_name);

    return 0;
}


