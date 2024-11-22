#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <time.h>
#include "weather.h"
#include "email.h"

int main() {
    CURL *curl;
    CURLcode res;
    float temperature, humidity;

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set API URL (replace YOUR_API_KEY with your actual API key)
        const char *apiUrl = "https://api.openweathermap.org/data/2.5/weather?q=Karachi&appid=0d22435c10bb9be4b4e18e9e1f6559c6";

        // Struct to store API response
        struct MemoryStruct chunk;
        chunk.memory = malloc(1);  // Initialize with an empty string
        chunk.size = 0;

        // Set CURL options
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Null-terminate the API response
            chunk.memory[chunk.size] = '\0';

            // Store raw data in a file
            FILE *rawDataFile = fopen("raw_data.txt", "a");
            fprintf(rawDataFile, "Raw Data:\n%s\n", chunk.memory);
            fclose(rawDataFile);

            // Process and analyze the data
            processData(chunk.memory);

            // Generate a report based on the analyzed data
            generateReport();
        }

        // Cleanup
        free(chunk.memory);
        curl_easy_cleanup(curl);
    }

    // Cleanup global CURL environment
    curl_global_cleanup();

    return 0;
}

