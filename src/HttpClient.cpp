#include "HttpClient.h"

HttpClient::HttpClient(){

    curl_global_init(CURL_GLOBAL_DEFAULT);

}

HttpClient::~HttpClient(){

    curl_global_cleanup();
}


nlohmann::json HttpClient::Get(const std::string& url){

    std::string response;

    //Initialize libcurl handle
    CURL* curl = curl_easy_init();

    if(curl){

        //Set URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        //Set callback to recieve data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        //GET request
        CURLcode res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);


    }

    //Parse JSON
    try{
        nlohmann::json jsonResponse = nlohmann::json::parse(response);
        return jsonResponse;
    } catch (const nlohmann::json::parse_error& e){
        std::cerr << "JSON parsing errjor: " << e.what() << std::endl;
        return{};
    }

}