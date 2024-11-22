#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <time.h>
#include "weather.h"
#include "email.h"


// Global variables to store weather information for analysis
float temperature, humidity;

// Function to get the current time and date
void getCurrentTime(char *timeString) {
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Callback function for writing data received from the API
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realSize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realSize + 1);

    if (mem->memory == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realSize);
    mem->size += realSize;
    mem->memory[mem->size] = 0;

    return realSize;
}

// Function for parsing JSON response and extracting weather information
void processData(const char *jsonResponse) {
    struct json_object *parsed_json;
    struct json_object *main;
    struct json_object *weather;
    struct json_object *wind;
    struct json_object *city;

    parsed_json = json_tokener_parse(jsonResponse);

    // Check if parsing was successful
    if (parsed_json == NULL) {
        fprintf(stderr, "Error parsing JSON response\n");
        return;
    }

    // Extract "main" object (temperature, humidity, min temp, max temp, pressure)
    json_object_object_get_ex(parsed_json, "main", &main);
    struct json_object *tempObj, *humidityObj;
    json_object_object_get_ex(main, "temp", &tempObj);
    json_object_object_get_ex(main, "humidity", &humidityObj);

    temperature = (float)json_object_get_double(tempObj);
    humidity = (float)json_object_get_double(humidityObj);

    // Extract "weather" array (description)
    json_object_object_get_ex(parsed_json, "weather", &weather);

    // Extract "wind" object (wind speed)
    json_object_object_get_ex(parsed_json, "wind", &wind);

    // Extract "name" (city name)
    json_object_object_get_ex(parsed_json, "name", &city);

    
    // Get the current time and date
    char timeString[26];
    getCurrentTime(timeString);
    
    
    // Print the weather data
    printf("\n*********************************************\n");
    printf("----Data generated on %s----\n", timeString);
    printf("*********************************************\n");
    printf("\nWeather Information:\n");
    printf("Temperature: %.2f°C\n", temperature-273.15);
    printf("Humidity: %.2f%%\n", humidity);
    printf("Min Temperature: %.2f°C\n", (float)json_object_get_double(json_object_object_get(main, "temp_min"))-273.15);
    printf("Max Temperature: %.2f°C\n", (float)json_object_get_double(json_object_object_get(main, "temp_max"))-273.15);
    printf("Pressure: %.2f hPa\n", (float)json_object_get_double(json_object_object_get(main, "pressure")));
    printf("Wind Speed: %.2f m/s\n", (float)json_object_get_double(json_object_object_get(wind, "speed")));
    printf("Weather Description: %s\n", json_object_get_string(json_object_object_get(json_object_array_get_idx(weather, 0), "description")));
    printf("City: %s\n", json_object_get_string(city));

    // Example algorithm: Check for high temperature or humidity anomalies
    // Define threshold values for anomalies
    float temperatureHighThreshold = 35.0;
    float temperatureLowThreshold = 20.0;
    float humidityThreshold = 80.0;

    // Check for high and low temperature anomaly
    if (temperature-273.15 > temperatureHighThreshold) {
        printf("\nHigh Temperature Anomaly Detected: %.2f > %.2f\n", temperature-273.15, temperatureHighThreshold);
    }
    if (temperature-273.15 < temperatureLowThreshold) {
        printf("\nLow Temperature Anomaly Detected: %.2f > %.2f\n", temperature-273.15, temperatureLowThreshold);
    } 
    else {
        printf("\nTemperature within normal range.\n");
    }

    // Check for high humidity anomaly
    if (humidity > humidityThreshold) {
        printf("High Humidity Anomaly Detected: %.2f > %.2f\n", humidity, humidityThreshold);
    } else {
        printf("Humidity within normal range.\n");
    }

    // Print the analysis result
    printf("\nAnalysis Result: Anomalies or Trends Detected\n");


    // Store weather information in a file
    
    FILE *processedDataFile = fopen("processed_data.txt", "a");
    
    // Print the current time and date in the processed data file
    fprintf(processedDataFile, "\n*********************************************\n");
    fprintf(processedDataFile, "Data generated on %s\n", timeString);
    fprintf(processedDataFile, "*********************************************\n");

    fprintf(processedDataFile, "Weather Information:\n");
    fprintf(processedDataFile, "Temperature: %.2f°C\n", temperature-273.15);
    fprintf(processedDataFile, "Humidity: %.2f%%\n", humidity);
    fprintf(processedDataFile, "Min Temperature: %.2f°C\n", (float)json_object_get_double(json_object_object_get(main, "temp_min"))-273.15);
    fprintf(processedDataFile, "Max Temperature: %.2f°C\n", (float)json_object_get_double(json_object_object_get(main, "temp_max"))-273.15);
    fprintf(processedDataFile, "Pressure: %.2f hPa\n", (float)json_object_get_double(json_object_object_get(main, "pressure")));
    fprintf(processedDataFile, "Wind Speed: %.2f m/s\n", (float)json_object_get_double(json_object_object_get(wind, "speed")));
    fprintf(processedDataFile, "Weather Description: %s\n", 	        json_object_get_string(json_object_object_get(json_object_array_get_idx(weather, 0), "description")));
    fprintf(processedDataFile, "City: %s\n\n", json_object_get_string(city));
    fclose(processedDataFile);
    
    
    char email_body[256]; 
    
    // Perform trend analysis and anomaly detection (example logic)
      if (temperature-273.15 > 35.0){

            printf("High temperature detected. Current temperature (%.2f)°C exceeds the temperature 35°C.\n", temperature-273.15);
            snprintf(email_body, sizeof(email_body),"High temperature detected. Current temperature (%.2f)°C exceeds the maximum temperature 35°C.\n", temperature-273.15);

        
         } else if (temperature-273.15 < 20.0) {
            printf("Low temperature detected. Current temperature (%.2f)°C is below the temperature 20°C.\n", temperature-273.15);
         
            snprintf(email_body, sizeof(email_body),"low temperature detected. Current temperature (%.2f)°C is below the minimum temperature 20°C.\n", temperature-273.15);

            
        } else {
       	    snprintf(email_body, sizeof(email_body),"Temperature detected. Current temperature (%.2f)°C is within normal range.\n", temperature-273.15);
        }

	send_email("zainabnoor021@gmail.com", email_body);
}

// Function to generate a report based on the analyzed data
void generateReport() {
     
    // Get the current time and date
    char timeString[26];
    getCurrentTime(timeString);

    // Open the report file in append mode
    FILE *reportFile = fopen("report.txt", "a");

    // Check if the report file is successfully opened
    if (reportFile == NULL) {
        fprintf(stderr, "Error opening report file\n");
        return;
    }

    // Print the current time and date in the report file
    fprintf(reportFile, "\n*********************************************\n");
    fprintf(reportFile, "Report generated on %s\n", timeString);
    fprintf(reportFile, "*********************************************\n");

    // Example algorithm: Check for high temperature or humidity anomalies
    // Define threshold values for anomalies
    float temperatureHighThreshold = 35.0;
    float temperatureLowThreshold = 20.0;
    float humidityThreshold = 80.0;

    // Check for high temperature anomaly
    if (temperature-273.15 > temperatureHighThreshold) {
        fprintf(reportFile, "High Temperature Anomaly Detected: %.2f > %.2f\n", temperature-273.15, temperatureHighThreshold);
    } 
    
    if (temperature-273.15 < temperatureLowThreshold) {
        fprintf(reportFile, "Low Temperature Anomaly Detected: %.2f > %.2f\n", temperature-273.15, temperatureLowThreshold);
    } 
    else {
        fprintf(reportFile, "Temperature within normal range.\n");
    }

    // Check for high humidity anomaly
    if (humidity > humidityThreshold) {
        fprintf(reportFile, "High Humidity Anomaly Detected: %.2f > %.2f\n", humidity, humidityThreshold);
    } else {
        fprintf(reportFile, "Humidity within normal range.\n");
    }

    // Print the analysis result
    fprintf(reportFile, "\nAnalysis Result: Anomalies or Trends Detected\n");

    fclose(reportFile);

    printf("\nReport generated successfully. Check 'report.txt' for details.\n");
}

