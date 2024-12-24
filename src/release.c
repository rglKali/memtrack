#include <stdlib.h>
#include <stdio.h>
#include "cell.h"
#include "env.h"

// Extern the global placeholder
extern _memtrack_env_t _memtrack_env_c;

// Release the environment
void _memtrack_release_f() {

    // Variables to track the memory
    size_t unreleased = 0, released = 0;

    // Print the header
    fprintf(stderr, "\033[1;39m=== result tracker ===\033[0m\n");

    // Iterate over the chunks
    for (size_t i = 0; i < _memtrack_env_c.size; i++) {

        // Check if the memory is released
        if (!_memtrack_env_c.chunks[i].used) {
            
            // Update the released counter
            released += _memtrack_env_c.chunks[i].size;

            // Continue to the next chunk
            continue;
        }

        // Print the missing free warning
        if (_memtrack_env_c.chunks[i].size == 0)
            fprintf(stderr, "\033[1;33mMissing free(%p) -> Zero lost\033[0m\n", 
            _memtrack_env_c.chunks[i].address);
        // Print the missing free warning
        else
            fprintf(stderr, "\033[1;31mMissing free(%p) -> %lub lost\033[0m\n", 
            _memtrack_env_c.chunks[i].address, _memtrack_env_c.chunks[i].size);

        // Free the memory
        free(_memtrack_env_c.chunks[i].address);

        // Update the unreleased memory
        unreleased += _memtrack_env_c.chunks[i].size;
    }

    // Print the summary
    fprintf(stderr, "\033[1;39mTotal bytes allocated: \033[1;34m%lub\033[0m\n", unreleased + released);
    fprintf(stderr, "\033[1;39mTotal bytes released: \033[1;32m%lub\033[0m\n", released);
    fprintf(stderr, "\033[1;39mTotal bytes forgotten: \033[1;31m%lub\033[0m\n", unreleased);

    // Free the chunks
    free(_memtrack_env_c.chunks);
}
