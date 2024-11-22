This project checks environmental data like temperature and humidity using a free weather API. It processes the data, finds any unusual readings, and creates reports. The system also sends email alerts when something unusual is detected or when the data goes back to normal.

Features:
Gets weather data from the OpenWeather API.
Processes data to find temperature and humidity issues.
Creates detailed reports from the data.
Sends email alerts when there are problems or when the data is back to normal.
Saves raw data and processed results in text files.
Installation:
Clone the repository.
Install the required libraries: libcurl and json-c.
Compile and run the code on a Linux system.
Set up email details in email.h.
Usage:
Run the program.
Choose options to get weather info, view processed data, or print reports.
