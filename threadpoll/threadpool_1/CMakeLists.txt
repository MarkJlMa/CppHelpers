file(GLOB source_code
    "*.h"
    "*.cpp"
)

add_executable(threadpool_1 ${source_code})
