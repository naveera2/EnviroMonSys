// email.h

#ifndef EMAIL_H
#define EMAIL_H


// Struct to hold email body information
struct EmailBodyInfo {
const char *data;
size_t size;
};

size_t ignore_output( void *ptr, size_t size, size_t nmemb, void *stream);
// Callback function to provide email body data
size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
// Function declarations
void send_email(const char *recipient, const char *body);

#endif // EMAIL_H

