#ifndef FOR_LOOP_H
#define FOR_LOOP_H

#include <stdbool.h>

// For loop direction
typedef enum {
    FOR_TO,       // for i = 0 to 10
    FOR_DOWNTO    // for i = 10 downto 0
} ForDirection;

// For loop structure
typedef struct ForLoop {
    char* var_name;              // Loop variable (e.g., "i")
    int start_value;             // Starting value
    int end_value;               // Ending value
    ForDirection direction;       // TO or DOWNTO
    struct Statement* body;      // Loop body statements
} ForLoop;

// Function declarations
void for_loop_free(ForLoop* loop);

#endif // FOR_LOOP_H
