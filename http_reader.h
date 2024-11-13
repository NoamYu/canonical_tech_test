#pragma once
#include <string>

/// @brief download a file from an https url
/// @param url https url 
/// @param outputPath where to download the file to 
/// @return True of the operation succeded
bool downloadFile(const std::string& url, const std::string& outputPath);
