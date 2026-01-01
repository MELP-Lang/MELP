#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE
#include "error_types.h"
#include "cJSON.h"

#define MAX_ERRORS 500

typedef struct {
    char code[16];
    ErrorMessage tr_msg;
    ErrorMessage en_msg;
} ErrorEntry;

static ErrorEntry error_database[MAX_ERRORS];
static int error_count = 0;

// Helper: Safe string duplicate
static char* safe_strdup(const char* str) {
    if (!str) return NULL;
    return strdup(str);
}

// Parse error message from JSON object
static void parse_error_message(cJSON* lang_obj, ErrorMessage* msg) {
    cJSON* title = cJSON_GetObjectItem(lang_obj, "title");
    cJSON* message = cJSON_GetObjectItem(lang_obj, "message");
    cJSON* help = cJSON_GetObjectItem(lang_obj, "help");
    cJSON* example = cJSON_GetObjectItem(lang_obj, "example");
    cJSON* severity = cJSON_GetObjectItem(lang_obj, "severity");

    msg->title = cJSON_IsString(title) ? safe_strdup(title->valuestring) : NULL;
    msg->message = cJSON_IsString(message) ? safe_strdup(message->valuestring) : NULL;
    msg->help = cJSON_IsString(help) ? safe_strdup(help->valuestring) : NULL;
    msg->example = cJSON_IsString(example) ? safe_strdup(example->valuestring) : NULL;
    msg->severity = cJSON_IsString(severity) ? safe_strdup(severity->valuestring) : safe_strdup("error");
}

// Load error database from JSON file
int load_error_database(const char* json_path) {
    FILE* fp = fopen(json_path, "rb");
    if (!fp) {
        return 0;
    }

    // Read entire file
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* json_buffer = (char*)malloc(file_size + 1);
    if (!json_buffer) {
        fclose(fp);
        return 0;
    }

    size_t read_size = fread(json_buffer, 1, file_size, fp);
    json_buffer[read_size] = '\0';
    fclose(fp);

    // Parse JSON
    cJSON* root = cJSON_Parse(json_buffer);
    free(json_buffer);

    if (!root) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) {
            fprintf(stderr, "JSON Parse Error: %s\n", error_ptr);
        }
        return 0;
    }

    // Iterate through all error codes
    cJSON* error_obj = NULL;
    cJSON_ArrayForEach(error_obj, root) {
        if (error_count >= MAX_ERRORS) break;

        const char* code = error_obj->string;
        if (!code || (code[0] != 'E' && code[0] != 'W')) continue;

        // Store code
        strncpy(error_database[error_count].code, code, sizeof(error_database[error_count].code) - 1);
        error_database[error_count].code[sizeof(error_database[error_count].code) - 1] = '\0';

        // Parse Turkish message
        cJSON* tr_obj = cJSON_GetObjectItem(error_obj, "tr");
        if (tr_obj) {
            parse_error_message(tr_obj, &error_database[error_count].tr_msg);
        }

        // Parse English message
        cJSON* en_obj = cJSON_GetObjectItem(error_obj, "en");
        if (en_obj) {
            parse_error_message(en_obj, &error_database[error_count].en_msg);
        }

        error_count++;
    }

    cJSON_Delete(root);
    return error_count;
}

// Get error message by code and language
ErrorMessage* get_error_message(const char* code, const char* lang) {
    for (int i = 0; i < error_count; i++) {
        if (strcmp(error_database[i].code, code) == 0) {
            ErrorMessage* msg = (ErrorMessage*)malloc(sizeof(ErrorMessage));
            if (!msg) return NULL;
            
            if (strcmp(lang, "en") == 0) {
                *msg = error_database[i].en_msg;
            } else {
                // Default to Turkish
                *msg = error_database[i].tr_msg;
            }
            
            // Ensure all fields are set
            if (!msg->title) msg->title = "";
            if (!msg->message) msg->message = "";
            if (!msg->help) msg->help = "";
            if (!msg->example) msg->example = "";
            if (!msg->severity) msg->severity = "error";
            
            return msg;
        }
    }
    return NULL;
}

// Free error message
void free_error_message(ErrorMessage* msg) {
    if (msg) {
        free(msg);
    }
}

// Cleanup database
void cleanup_error_database(void) {
    for (int i = 0; i < error_count; i++) {
        // Free TR messages
        if (error_database[i].tr_msg.title) free(error_database[i].tr_msg.title);
        if (error_database[i].tr_msg.message) free(error_database[i].tr_msg.message);
        if (error_database[i].tr_msg.help) free(error_database[i].tr_msg.help);
        if (error_database[i].tr_msg.example) free(error_database[i].tr_msg.example);
        if (error_database[i].tr_msg.severity) free(error_database[i].tr_msg.severity);
        
        // Free EN messages
        if (error_database[i].en_msg.title) free(error_database[i].en_msg.title);
        if (error_database[i].en_msg.message) free(error_database[i].en_msg.message);
        if (error_database[i].en_msg.help) free(error_database[i].en_msg.help);
        if (error_database[i].en_msg.example) free(error_database[i].en_msg.example);
        if (error_database[i].en_msg.severity) free(error_database[i].en_msg.severity);
    }
    error_count = 0;
}
