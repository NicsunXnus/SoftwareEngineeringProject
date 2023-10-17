file(GLOB SRC_DIR "Team18/Code18/source/**/*.cpp")
file(GLOB SRC_DIR_2 "Team18/Code18/source/**/**/*.cpp")
file(GLOB SRC_DIR_3 "Team18/Code18/source/*.cpp")
file(GLOB AUTOTESTER "Team18/Code18/AutoTester/source/*.cpp")
file(GLOB HDR_DIR "Team18/Code18/source/**/*.h")
file(GLOB HDR_DIR_2 "Team18/Code18/source/*.h")
file(GLOB HDR_DIR_3 "Team18/Code18/source/**/**/*.h")
file(GLOB UNIT_TEST_SOURCES "Team18/Code18/UnitTesting/TestPQLTokenizer.cpp")
file(GLOB SYSTEM_TEST_SOURCES "Team18/Code18/SystemTesting/SystemTestMain.cpp")
file(GLOB SYSTEM_TEST_FILES "Team18/Tests18/Milestone1/*.txt")

set(sources
    ${SRC_DIR}
    ${SRC_DIR_2}
    ${SRC_DIR_3}
)

set(exe_sources
	${sources}
	${AUTOTESTER}
)

set (sys_tests_files
    ${SYSTEM_TEST_FILES}
)

set(headers
    ${HDR_DIR}
    ${HDR_DIR_2}
    ${HDR_DIR_3}
)

set(test_sources
  ${UNIT_TEST_SOURCES}
)

set(system_test_sources
  ${SYSTEM_TEST_SOURCES}
)