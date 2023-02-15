file(GLOB source_code
    "*.h"
    "*.cpp"
)

add_executable(threadpool_ocpp ${source_code})
