#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <string>
#include "http_reader.h"

size_t WriteCallback(void* ptr, size_t size, size_t nmemb, void* data) {
    std::ofstream* stream = static_cast<std::ofstream*>(data);
    size_t written = size * nmemb;
    stream->write(static_cast<char*>(ptr), written);
    return written;
}

bool downloadFile(const std::string& url, const std::string& outputPath) {
    CURL* curl;
    CURLcode res;
    std::ofstream file;

    // Initialize libcurl
    curl = curl_easy_init();
    if (curl) {
        // Open file to write to
        file.open(outputPath, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file: " << outputPath << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Follow redirections if any
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Set the write callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Set data to pass to the write function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

        // Perform the download
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        file.close();
        return true;
    } else {
        std::cerr << "Failed to initialize curl" << std::endl;
        return false;
    }
}

// Function to download JSON data as a string
bool downloadJson(const std::string& url, std::string& jsonString) {
    CURL* curl;
    CURLcode res;
    jsonString.clear();

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &jsonString);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        curl_easy_cleanup(curl);
        return true;
    }
    return false;
}
