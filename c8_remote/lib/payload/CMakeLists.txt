include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
set(CMAKE_SYSTEM_PROCESSOR arm)

# regular desktop
set(CMAKE_C_COMPILER    /opt/aarch64-none-elf/bin/aarch64-none-elf-gcc)
set(CMAKE_ASM_COMPILER  /opt/aarch64-none-elf/bin/aarch64-none-elf-as)
set(CMAKE_OBJCOPY       /opt/aarch64-none-elf/bin/aarch64-none-elf-objcopy)
set(CMAKE_RANLIB        /opt/aarch64-none-elf/bin/aarch64-none-elf-ranlib)

set(CMAKE_C_FLAGS "-nostdlib -O")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

foreach(NAME ${PL_NAMES})
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/${NAME}.S)
        add_executable(payload_${NAME} ${CMAKE_CURRENT_LIST_DIR}/payload_entry.S
                                        ${CMAKE_CURRENT_LIST_DIR}/payload_entry.c
                                        ${CMAKE_CURRENT_LIST_DIR}/src/${NAME}.c
                                        ${CMAKE_CURRENT_LIST_DIR}/src/${NAME}.S)
    else()
        add_executable(payload_${NAME} ${CMAKE_CURRENT_LIST_DIR}/payload_entry.S
                                        ${CMAKE_CURRENT_LIST_DIR}/payload_entry.c
                                        ${CMAKE_CURRENT_LIST_DIR}/src/${NAME}.c)
    endif()

    add_custom_command(TARGET       payload_${NAME} POST_BUILD
                        BYPRODUCTS  ${CMAKE_CURRENT_BINARY_DIR}/bin/payload_${NAME}.bin
                        COMMAND     ${CMAKE_OBJCOPY}
                        ARGS        -O binary -j .text -j .payload_text
                                    ${CMAKE_CURRENT_BINARY_DIR}/payload_${NAME}
                                    ${CMAKE_CURRENT_BINARY_DIR}/bin/payload_${NAME}.bin)
endforeach(NAME)
