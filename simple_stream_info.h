#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class SimpleStreamInfo
{
private:
    nlohmann::json m_simple_stream_data;
    nlohmann::json_abi_v3_11_3::json getLTSProducts() const;
    void filterArch(std::string arch_name);
public:
    SimpleStreamInfo(const std::string& stream_url);
    ~SimpleStreamInfo() =default;

    void GetSupportedReleases(std::vector<std::string>& supported_versions) const;
    std::string GetLatestLTSRelease() const;
    bool GetShaOfDisk1(const std::string& version_name, const std::string& specific_version, std::string& sha_value) const;
};