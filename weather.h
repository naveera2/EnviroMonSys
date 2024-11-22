// weather.h

#ifndef WEATHER_H
#define WEATHER_H

#include <json-c/json.h>

// Struct to store the API response
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Function declarations
void processData(const char *jsonResponse);
void getCurrentTime(char *timeString);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void generateReport();

#endif // WEATHER_H

