function(configure_tidy TARGET_NAME)
    if (VARIANT_POINTER_CLANG_TIDY)
        clang_tidy_check(${TARGET_NAME})
    endif()
endfunction()