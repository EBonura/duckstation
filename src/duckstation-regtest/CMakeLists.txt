add_executable(duckstation-regtest
  regtest_host.cpp
)

target_include_directories(duckstation-regtest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(duckstation-regtest PRIVATE core common scmversion)

# Skip add_core_resources when Metal compiler is unavailable (no Xcode).
# Regtest only uses software GPU, so Metal shaders are not needed.
find_program(XCRUN_METAL xcrun)
execute_process(COMMAND xcrun --find metal RESULT_VARIABLE METAL_RESULT OUTPUT_QUIET ERROR_QUIET)
if(METAL_RESULT EQUAL 0)
  add_core_resources(duckstation-regtest)
else()
  message(STATUS "Metal compiler not found — skipping Metal shaders for regtest (software GPU only)")
  # Still add non-Metal resources
  add_resources(duckstation-regtest "resources" ${CMAKE_SOURCE_DIR}/data/resources)
endif()