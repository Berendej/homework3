cmake_minimum_required( VERSION 3.10 )
project( homework3 VERSION 0.1.1 )
add_executable( alloc allocator.cpp  )
add_executable( test_alloc test_alloc.cpp  )

# package part which enables
# run: cmake --build . --target package
install(TARGETS test_alloc RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_CONTACT berrendej@gmail.com)
include(CPack)

#testing part which enables
# cmake --build . --target test
enable_testing()
add_test(stack_alloc_test  test_alloc) 