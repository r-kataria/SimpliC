#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "server.h"

// Structure to represent a rewrite rule
typedef struct RewriteRule {
    // Incoming request
    char *new_path;

    // Actual file
    char *actual_path;
    struct RewriteRule *next;
} RewriteRule;

// To load rewrite rules from the configuration file
RewriteRule* load_rewrite_rules(const char *config_path);

// To find a rewrite rule matching the given path
const char* apply_rewrite(RewriteRule *rules, const char *path);

#endif // FILE_LOADER_H
