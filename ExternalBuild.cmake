if(WIN32)
    set(PD_PATH "$ENV{ProgramFiles}/Pd")
    set(COMPILER_FLAG "/DPD_LONGINTTYPE=\"long long\" /wd4091 /wd4996 /wd4189")
else(APPLE)
    set(PD_PATH "/Applications/Pd-0.50-0.app/Contents/Resources")
endif()

function(add_external EXTERNAL_NAME)
    string(REPLACE "_tilde" "~" EXTERNAL_OBJECT_NAME ${EXTERNAL_NAME})
    add_library(${EXTERNAL_NAME} SHARED "source/${EXTERNAL_OBJECT_NAME}.c")
    target_include_directories(${EXTERNAL_NAME} PRIVATE "${PD_PATH}/src" include)
    target_link_libraries(${EXTERNAL_NAME} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:"${PD_PATH}/bin/pd">
            $<$<CXX_COMPILER_ID:MSVC>:kernel32.lib>
            $<$<CXX_COMPILER_ID:MSVC>:ws2_32.lib>
            $<$<CXX_COMPILER_ID:MSVC>:advapi32.lib>
            $<$<CXX_COMPILER_ID:MSVC>:userenv.lib>
            DSP::DSP)
    set_target_properties(${EXTERNAL_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${PD_OUTPUT_PATH}
            LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PD_OUTPUT_PATH}
            LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PD_OUTPUT_PATH}
            RUNTIME_OUTPUT_DIRECTORY ${PD_OUTPUT_PATH}
            RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PD_OUTPUT_PATH}
            RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PD_OUTPUT_PATH})

    if(WIN32)
        set_target_properties(${EXTERNAL_NAME} PROPERTIES
                SUFFIX ".dll"
                COMPILE_FLAGS ${COMPILER_FLAG}
                OUTPUT_NAME ${EXTERNAL_OBJECT_NAME}
                LINK_FLAGS "/export:${EXTERNAL_NAME}_setup")
    elseif(APPLE)
        set_target_properties(${EXTERNAL_NAME} PROPERTIES
                PREFIX ""
                SUFFIX ".pd_darwin"
                OUTPUT_NAME ${EXTERNAL_OBJECT_NAME}
                LINK_FLAGS "-undefined dynamic_lookup")
    endif()
endfunction()
