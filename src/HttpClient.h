#include <iostream>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class HttpClient {

    public:

        HttpClient();

        ~HttpClient();


        nlohmann::json Get(const std::string& url);

    private:

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output){
        size_t totalSize = size * nmemb;
        output->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

        

};