cmake_minimum_required(VERSION 3.10)


set(PD_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/externals")
include(ExternalBuild.cmake)

set(PdExternalList
        rp_additive_tilde
        rp_delay_tilde
        rp_hold_tilde
        rp_phasor_tilde
        rp_test_tilde
        rp_wintp_tilde
        rp_xfader_tilde
        rp_zerox_tilde
  )

foreach(PdExternal IN LISTS PdExternalList)
    add_external(${PdExternal})
endforeach()

add_custom_target(PdExternals)
add_dependencies(PdExternals ${PdExternalList})
