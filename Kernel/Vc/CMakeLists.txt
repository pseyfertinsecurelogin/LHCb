################################################################################
# Package: Vc
################################################################################
gaudi_subdir(Vc v1r3)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

gaudi_depends_on_subdirs(GaudiPolicy)

include(ExternalProject)

set(VC_URL https://github.com/VcDevel/Vc.git)
set(VC_VERSION f55bb44)

ExternalProject_Add(Vc
   GIT_REPOSITORY ${VC_URL}
   GIT_TAG        ${VC_VERSION}
   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
              -DBUILD_TESTING=OFF
              -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
              -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
   PATCH_COMMAND sed -i "s|\@CMAKE_INSTALL_PREFIX\@|\${CMAKE_CURRENT_LIST_DIR}/../..|g" cmake/VcConfig.cmake.in && sed -i "s|cmake/Vc|../cmake/Vc|g" cmake/VcConfig.cmake.in
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/include/Vc DESTINATION include)
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/cmake/Vc DESTINATION cmake)
install(FILES     ${CMAKE_BINARY_DIR}/lib/libVc.a DESTINATION lib)
