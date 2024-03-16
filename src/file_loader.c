#include "file_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to trim whitespace from a string (helper function)
char* trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while(*str == ' ' || *str == '\t') str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str;
}

// Function to load rewrite rules from the configuration file
RewriteRule* load_rewrite_rules(const char *config_path) {
    FILE *file = fopen(config_path, "r");
    if (!file) {
        perror("Failed to open rewrites.conf");
        return NULL;
    }

    RewriteRule *head = NULL;
    RewriteRule *tail = NULL;
    char line[512];

    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || strlen(line) < 3)
            continue;

        char new_path[256], actual_path[256];
        if (sscanf(line, "%s %s", new_path, actual_path) != 2)
            continue;

        // Allocate a new RewriteRule
        RewriteRule *rule = (RewriteRule *)malloc(sizeof(RewriteRule));
        if (!rule) {
            perror("Failed to allocate memory for RewriteRule");
            fclose(file);
            return head;
        }

        rule->new_path = strdup(new_path);
        rule->actual_path = strdup(actual_path);
        rule->next = NULL;

        // Append to the linked list
        if (tail == NULL) {
            head = tail = rule;
        } else {
            tail->next = rule;
            tail = rule;
        }

        // Log the loaded rule
        printf("Loaded rewrite rule: %s -> %s\n", rule->new_path, rule->actual_path);
    }

    fclose(file);
    return head;
}

// Function to apply rewrite rules to a given path
const char* apply_rewrite(RewriteRule *rules, const char *path) {
    RewriteRule *current = rules;
    while (current) {
        // Handle exact matches
        if (strcmp(current->new_path, path) == 0) {
            return current->actual_path;
        }

        // Handle wildcard rewrites
        char *wildcard_pos = strchr(current->new_path, '*');
        if (wildcard_pos) {
            // Extract the prefix before '*'
            size_t prefix_len = wildcard_pos - current->new_path;
            if (strncmp(current->new_path, path, prefix_len) == 0) {
                // Extract the suffix from the request path
                const char *suffix = path + prefix_len;
                // Construct the actual path by replacing '*' with the suffix
                static char actual_full_path[512];
                snprintf(actual_full_path, sizeof(actual_full_path), "%s%s", current->actual_path, suffix);
                return actual_full_path;
            }
        }

        current = current->next;
    }
    return NULL; // No rewrite rule matched
}
