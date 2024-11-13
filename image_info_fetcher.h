# include<vector>
// Interface class declaration
class IImageInfoFetcher {
public:
    virtual ~IImageInfoFetcher() = default;

    virtual void GetSupportedReleases(std::vector<std::string>& supported_versions) const = 0;
    virtual std::string GetLatestLTSRelease() const = 0;
    virtual bool GetShaOfDisk1(const std::string& version_name, const std::string& specific_version, std::string& sha_value) const = 0;
};