################################################################################
# Package: CaloDetXmlCnv
################################################################################
gaudi_subdir(CaloDetXmlCnv v3r3p1)

find_package(Boost)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS})

gaudi_depends_on_subdirs(Det/CaloDet
                         Det/DetDescCnv)

gaudi_add_module(CaloDetXmlCnv
                 src/*.cpp
                 LINK_LIBRARIES CaloDetLib DetDescCnvLib)

