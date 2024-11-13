#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include "simple_stream_info.h"
#include "http_reader.h"


int main(int argc, char* argv[]) {
    const std::string url = "https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json";
    SimpleStreamInfo sample_stream(url);

    if (argc < 2) {
        std::cerr << "Usage: app -supported | -LTS | -sha <version_name> <sub_version>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "-supported") {
        std::vector<std::string> supported_versions;
        sample_stream.GetSupportedReleases(supported_versions);

        std::cout << "Supported Ubuntu releases: ";
        for (const auto& version : supported_versions) {
            std::cout << version << " ";
        }
        std::cout << std::endl;
    } 
    else if (command == "-LTS") {
        std::cout << "Latest Ubuntu LTS release: " << sample_stream.GetLatestLTSRelease() << std::endl;
    } 
    else if (command == "-sha") {
        if (argc != 4) {
            std::cerr << "Usage: app -sha <version_name> <sub_version>" << std::endl;
            return 1;
        }

        std::string ubuntu_release_name = argv[2];
        std::string specific_version = argv[3];
        std::string sha_value;
        
        sample_stream.GetShaOfDisk1(ubuntu_release_name, specific_version, sha_value);
        
        std::cout << "SHA for " << ubuntu_release_name << " version " << specific_version << ": " << sha_value << std::endl;
    } 
    else {
        std::cerr << "Invalid command. Use -supported, -LTS, or -sha." << std::endl;
        return 1;
    }

    return 0;
}
