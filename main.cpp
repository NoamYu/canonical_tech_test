#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include "simple_stream_info.h"
#include "http_reader.h"



int main() {
    const std::string url = "https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json";
    
    SimpleStreamInfo sample_stream(url);
    std::vector<std::string> supported_versions = {};
    // std::cout << "Parsed JSON Data:\n";
    // std::cout << data.dump(4) << std::endl;

    sample_stream.GetSupportedReleases(supported_versions);
    for(const auto& version : supported_versions)
        std::cout << version << std::endl;

    std::cout << "latest ubuntu release " << sample_stream.GetLatestLTSRelease() << std::endl;

    std::string ubuntu_release_name = "com.ubuntu.cloud:server:17.04:amd64";
    std::string specific_version = "20171208";
    std::string sha_value;
    sample_stream.GetShaOfDisk1(ubuntu_release_name, specific_version, sha_value);
    std::cout << sha_value;

    return 0;
}


