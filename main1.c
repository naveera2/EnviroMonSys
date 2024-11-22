#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <time.h>
#include "weather.h"
#include "email.h"

// Function to print processed data
void getProcessedData() {
    // Retrieve and print processed data from the processed data file
    FILE *processedDataFile = fopen("processed_data.txt", "r");

    if (processedDataFile == NULL) {
        fprintf(stderr, "Error opening processed data file\n");
        return;
    }

    char ch;
    while ((ch = fgetc(processedDataFile)) != EOF) {
        putchar(ch);
    }

    fclose(processedDataFile);
}

// Function to print the report
void printReport() {
    // Retrieve and print the report from the report file
    FILE *reportFile = fopen("report.txt", "r");

    if (reportFile == NULL) {
        fprintf(stderr, "Error opening report file\n");
        return;
    }

    char ch;
    while ((ch = fgetc(reportFile)) != EOF) {
        putchar(ch);
    }

    fclose(reportFile);
}

// Function to print the main menu
void printMenu() {
    printf("\nMain Menu:\n");
    printf("1. Get Weather Info\n");
    printf("2. Get Processed Data\n");
    printf("3. Print Report\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main() {
    CURL *curl;
    CURLcode res;
    float temperature, humidity;

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "CURL initialization failed.\n");
        return 1;
    }

    int choice;
    do {
        // Print the menu
        printMenu();

        // Get user input
        scanf("%d", &choice);

        switch (choice) {
            case 1:
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
                    
                    // Generate report
                    generateReport();

                }

                // Cleanup
                free(chunk.memory);
                break;

            case 2:
                // Get processed data
                printf("\nGetting Processed Data...\n");
                getProcessedData();
                break;

            case 3:
                // Print report
                printf("\nPrinting Report...\n");
                printReport();
                break;

            case 4:
                // Exit the program
                printf("\nExiting the program. Goodbye!\n");
                break;

            default:
                printf("\nInvalid choice. Please enter a number between 1 and 4.\n");
        }

    } while (choice != 4);

    // Cleanup global CURL environment
    curl_global_cleanup();

    return 0;
}

