#ifndef FOR_LOOP_H
#define FOR_LOOP_H

#include <stdbool.h>

// For loop direction
typedef enum {
    FOR_TO,       // for i = 0 to 10
    FOR_DOWNTO    // for i = 10 downto 0
} ForDirection;

// For loop type
typedef enum {
    FOR_TYPE_RANGE,     // for i = 0 to 10
    FOR_TYPE_EACH       // for each item in collection
} ForLoopType;

// For loop structure
typedef struct ForLoop {
    ForLoopType loop_type;       // YZ_28: Range or each
    char* var_name;              // Loop variable (e.g., "i" or "item")
    int start_value;             // Starting value (range loop)
    int end_value;               // Ending value (range loop)
    ForDirection direction;       // TO or DOWNTO (range loop)
    char* collection_name;       // YZ_28: Collection variable name (for-each)
    struct Statement* body;      // Loop body statements
} ForLoop;

// Function declarations
void for_loop_free(ForLoop* loop);

#endif // FOR_LOOP_H
