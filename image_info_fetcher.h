# include<vector>


/// @brief An interface for fetching images information 
class IImageInfoFetcher {
public:
    virtual ~IImageInfoFetcher() = default;

    /// @brief Get a list of versions that are still supported 
    /// @param supported_versions content will be erased and a list of supported versions will be added.
    virtual void GetSupportedReleases(std::vector<std::string>& supported_versions) const = 0;

    /// @brief return the version number fo the latest LTS version
    /// @return string with the version number 
    virtual std::string GetLatestLTSRelease() const = 0;

    /// @brief retrun the sha of a specific image
    /// @param version_name the full name of the version, e.g. com.ubuntu.cloud:server:17.04:amd64
    /// @param sub_version the sub version, e.g. 20171208
    /// @param sha_value will be filled with the sha value of the image
    /// @return True if found the sha successfully.
    virtual bool GetShaOfDisk1(const std::string& version_name, const std::string& sub_version, std::string& sha_value) const = 0;
};