cmake_minimum_required( VERSION 2.6 )


project( ExploratorySliderTest )


#######################################
# Include VTK
#######################################

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

set( VTK_LIBS QVTK ) 


#######################################
# Include Qt
#######################################

# Use what QVTK built with
set( QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "" )
set( QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "" )
set( QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "" )
find_package( Qt )
if( QT_USE_FILE )
  include( ${QT_USE_FILE} )
else( QT_USE_FILE )
  set( QT_LIBRARIES ${QT_QT_LIBRARY} )
endif( QT_USE_FILE )

# Use the include path and library for Qt that is used by VTK.
include_directories( ${QT_INCLUDE_DIR} )

# Set up variables for moc
set( QT_UI MainWindow.ui )
set( QT_QRC ExploratorySliderTest.qrc )
set( QT_HEADER MainWindow.h )
set( QT_SRC ExploratorySliderTest.cpp MainWindow.cpp )

# Do moc stuff
qt4_wrap_ui( QT_UI_HEADER ${QT_UI} )
qt4_add_resources( QT_RCC_SRC ${QT_QRC} )
qt4_wrap_cpp( QT_MOC_SRC ${QT_HEADER} )
add_definitions( -DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT )
set_source_files_properties( ${QT_SRC} PROPERTIES OBJECT_DEPENDS "${QT_UI_HEADER}" )


#######################################
# Include QScientific
#######################################

find_package( QScientific REQUIRED )
include( ${QScientific_USE_FILE} )


#######################################
# Include ExploratorySliderTest code
#######################################

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set( SRC VTKPipeline.h VTKPipeline.cpp
         vtkImageTestDataSource.h vtkImageTestDataSource.cxx
         vtkMyInteractorStyleTrackballCamera.h vtkMyInteractorStyleTrackballCamera.cxx )
		 
# Add resource file on Windows		 
if( WIN32 ) 
  set( SRC ExploratorySliderTest.rc ${SRC} )
endif( WIN32 )		 

set( SHADERS perPixelLighting.xml )
source_group(Shaders FILES ${SHADERS} )

add_executable( ExploratorySliderTest ${QT_HEADER} ${QT_RCC_SRC} ${QT_SRC} ${QT_MOC_SRC} ${SRC} ${SHADERS} )
target_link_libraries( ExploratorySliderTest ${VTK_LIBS} ${QT_LIBRARIES} ${QScientific_LIB} )


#######################################
# Set installation package properties
#######################################

# Set the version string
set( ExploratorySliderTest_VERSION_STRING "1.0" )

# Check for 32 or 64-bit
if( ${CMAKE_SIZEOF_VOID_P} MATCHES "^8$" )
  set( ARCH_BITS "64" )
else( ${CMAKE_SIZEOF_VOID_P} MATCHES "^8$" )
  set( ARCH_BITS "32" )
endif( ${CMAKE_SIZEOF_VOID_P} MATCHES "^8$" )

set( CPACK_PACKAGE_DIRECTORY "${ExploratorySliderTest_SOURCE_DIR}/Installers" )
set( CPACK_PACKAGE_NAME "ExploratorySliderTest_${ARCH_BITS}" )
set( CPACK_PACKAGE_VENDOR "RENCI" )
set( CPACK_PACKAGE_EXECUTABLES "ExploratorySliderTest;ExploratorySliderTest ${ExploratorySliderTest_VERSION_STRING}" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "ExploratorySliderTest Volume Visualization Tool" )
set( CPACK_PACKAGE_VERSION "${ExploratorySliderTest_VERSION_STRING}" )
set( CPACK_PACKAGE_VERSION_MAJOR "${ExploratorySliderTest_MAJOR_NUMBER}" )
set( CPACK_PACKAGE_VERSION_MINOR "${ExploratorySliderTest_MINOR_NUMBER}" )
set( CPACK_PACKAGE_VERSION_PATCH "${ExploratorySliderTest_REVISION_NUMBER}" )
set( CPACK_PACKAGE_INSTALL_DIRECTORY "ExploratorySliderTest_${ExploratorySliderTest_VERSION_STRING}" )
set( CPACK_PACKAGE_INSTALL_REGISTRY_KEY "ExploratorySliderTest ${ExploratorySliderTest_VERSION_STRING}" )
#set( CPACK_RESOURCE_FILE_LICENSE "${ExploratorySliderTest_SOURCE_DIR}/License.txt" )
#set( CPACK_RESOURCE_FILE_README  "${ExploratorySliderTest_SOURCE_DIR}/README.txt" )

if( WIN32 )
  # Windows-specific CPACK instructions for NSIS
  set( CPACK_GENERATOR "NSIS" )
  
  set( CPACK_NSIS_URL_INFO_ABOUT "http://www.renci.org" )
  set( CPACK_NSIS_CONTACT "borland@renci.org" )

  # Both of these need to be set or neither of them is set.
  set( CPACK_NSIS_MUI_ICON "${ExploratorySliderTest_SOURCE_DIR}/Data/ExploratorySliderTest.ico" )
  set( CPACK_NSIS_MUI_UNIICON "${ExploratorySliderTest_SOURCE_DIR}/Data/ExploratorySliderTestUninstall.ico" )
  
  set( CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\ExploratorySliderTest.exe" )
  
  get_filename_component( QT_ROOT ${QT_QMAKE_EXECUTABLE} PATH )
  
  install( FILES ${QT_ROOT}/QtCore4.dll ${QT_ROOT}/QtGui4.dll
           DESTINATION bin )
endif( WIN32 )

# Setting the destination to bin makes a few other things much smoother, such as InstallRequiredSystemLibraries
install( TARGETS ExploratorySliderTest
         RUNTIME DESTINATION bin )

#install( FILES ${ExploratorySliderTest_SOURCE_DIR}/README.txt ${ExploratorySliderTest_SOURCE_DIR}/License.txt
#         DESTINATION . )

# This includes Microsoft runtime libraries when run in Windows environment.
include( InstallRequiredSystemLibraries )

# This must always be last!
include( CPack )
