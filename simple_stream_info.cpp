#include <fstream>
#include <iostream>
#include <sstream>
#include "http_reader.h"
#include "simple_stream_info.h"

using namespace std;
using namespace nlohmann;

SimpleStreamInfo::SimpleStreamInfo(const string& stream_url)
{
    std::string local_download_path = "simple_stream_file.json";
    try
    {
        downloadFile(stream_url, local_download_path);
        ifstream jasonFile(local_download_path);
        m_simple_stream_data = json::parse(jasonFile);
        filterArch("amd64");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void SimpleStreamInfo::filterArch(string arch_name)
{
    const auto lts_string_identfier = "LTS";
    auto& products_list = m_simple_stream_data["products"];
    for (auto it = products_list.begin(); it != products_list.end();)
    {
        if ((*it)["arch"] != arch_name)
        {
            it = products_list.erase(it); // Erase returns the next iterator
            continue;
        }
        ++it;
    }
}

void SimpleStreamInfo::GetSupportedReleases(std::vector<std::string>& supported_versions) const
{
    cout << "supported ubuntu releases\n";
    supported_versions.clear();
    for (auto product : m_simple_stream_data["products"])
    {
        if(product["supported"])
            supported_versions.push_back(product["version"]);
    }   
}

int compareVersions(const std::string& v1, const std::string& v2) 
{
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

std::string SimpleStreamInfo::GetShaOfDisk1(const std::string& version_name, const std::string& specific_version) const
{
    try
    {
        auto product_version = m_simple_stream_data["products"].find(version_name);
        if (product_version == m_simple_stream_data["products"].end())
        {
            return "Failed To Find sha for this specific version"; 
        }
        return m_simple_stream_data["products"][version_name]["versions"][specific_version]["items"]["disk1.img"]["sha256"];
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return "Failed To Find sha for this specific version";
    }
}

std::string SimpleStreamInfo::GetLatestLTSRelease() const
{
    auto products_list = getLTSProducts();

    auto max_version = std::max_element(
        products_list.begin(), products_list.end(),
        [](const auto& a, const auto& b) {
            return compareVersions(a["version"], b["version"]) < 0; // Compare by version number
        }
    );

    return (*max_version)["version"];
}

nlohmann::json_abi_v3_11_3::json SimpleStreamInfo::getLTSProducts() const
{
    const auto lts_string_identfier = "LTS";
    auto products_list = m_simple_stream_data["products"];
    for (auto it = products_list.begin(); it != products_list.end();)
    {
        string version = (*it)["release_title"];
        if (version.find(lts_string_identfier) == std::string::npos)
        {
            it = products_list.erase(it); // Erase returns the next iterator
            continue;
        }
        ++it;
    }
    return products_list;
}
