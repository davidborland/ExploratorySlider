cmake_minimum_required( VERSION 2.6 )


project( ExploratorySliderTest2 )


#######################################
# Include Qt
#######################################

find_package( Qt4 REQUIRED )

include( ${QT_USE_FILE} )

include_directories( ${QT_INCLUDE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR} )

# Set up variables for moc
set( QT_UI MainWindow.ui )
set( QT_QRC ExploratorySliderTest2.qrc )
set( QT_HEADER MainWindow.h )
set( QT_SRC ExploratorySliderTest2.cpp MainWindow.cpp )

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
# Include ExploratorySliderTest2 code
#######################################

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set( SRC )
		 
# Add resource file on Windows		 
if( WIN32 ) 
  set( SRC ExploratorySliderTest2.rc ${SRC} )
endif( WIN32 )

add_executable( ExploratorySliderTest2 ${QT_HEADER} ${QT_RCC_SRC} ${QT_SRC} ${QT_MOC_SRC} ${SRC} ${SHADERS} )
target_link_libraries( ExploratorySliderTest2 ${QT_LIBRARIES} ${QScientific_LIB} )


#######################################
# Set installation package properties
#######################################

# Set the version string
set( ExploratorySliderTest2_VERSION_STRING "1.0" )

# Check for 32 or 64-bit
if( ${CMAKE_SIZEOF_VOID_P} MATCHES "^8$" )
  set( ARCH_BITS "64" )
else( ${CMAKE_SIZEOF_VOID_P} MATCHES "^8$" )
  set( ARCH_BITS "32" )
endif( ${CMAKE_SIZEOF_VOID_P} MATCHES "^8$" )

set( CPACK_PACKAGE_DIRECTORY "${ExploratorySliderTest2_SOURCE_DIR}/Installers" )
set( CPACK_PACKAGE_NAME "ExploratorySliderTest2_${ARCH_BITS}" )
set( CPACK_PACKAGE_VENDOR "RENCI" )
set( CPACK_PACKAGE_EXECUTABLES "ExploratorySliderTest2;ExploratorySliderTest2 ${ExploratorySliderTest2_VERSION_STRING}" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "ExploratorySliderTest2 Volume Visualization Tool" )
set( CPACK_PACKAGE_VERSION "${ExploratorySliderTest2_VERSION_STRING}" )
set( CPACK_PACKAGE_VERSION_MAJOR "${ExploratorySliderTest2_MAJOR_NUMBER}" )
set( CPACK_PACKAGE_VERSION_MINOR "${ExploratorySliderTest2_MINOR_NUMBER}" )
set( CPACK_PACKAGE_VERSION_PATCH "${ExploratorySliderTest2_REVISION_NUMBER}" )
set( CPACK_PACKAGE_INSTALL_DIRECTORY "ExploratorySliderTest2_${ExploratorySliderTest2_VERSION_STRING}" )
set( CPACK_PACKAGE_INSTALL_REGISTRY_KEY "ExploratorySliderTest2 ${ExploratorySliderTest2_VERSION_STRING}" )
#set( CPACK_RESOURCE_FILE_LICENSE "${ExploratorySliderTest2_SOURCE_DIR}/License.txt" )
#set( CPACK_RESOURCE_FILE_README  "${ExploratorySliderTest2_SOURCE_DIR}/README.txt" )

if( WIN32 )
  # Windows-specific CPACK instructions for NSIS
  set( CPACK_GENERATOR "NSIS" )
  
  set( CPACK_NSIS_URL_INFO_ABOUT "http://www.renci.org" )
  set( CPACK_NSIS_CONTACT "borland@renci.org" )

  # Both of these need to be set or neither of them is set.
  set( CPACK_NSIS_MUI_ICON "${ExploratorySliderTest2_SOURCE_DIR}/Data/ExploratorySliderTest2.ico" )
  set( CPACK_NSIS_MUI_UNIICON "${ExploratorySliderTest2_SOURCE_DIR}/Data/ExploratorySliderTest2Uninstall.ico" )
  
  set( CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\ExploratorySliderTest2.exe" )
  
  get_filename_component( QT_ROOT ${QT_QMAKE_EXECUTABLE} PATH )
  
  install( FILES ${QT_ROOT}/QtCore4.dll ${QT_ROOT}/QtGui4.dll
           DESTINATION bin )
endif( WIN32 )

# Setting the destination to bin makes a few other things much smoother, such as InstallRequiredSystemLibraries
install( TARGETS ExploratorySliderTest2
         RUNTIME DESTINATION bin )

#install( FILES ${ExploratorySliderTest2_SOURCE_DIR}/README.txt ${ExploratorySliderTest2_SOURCE_DIR}/License.txt
#         DESTINATION . )

# This includes Microsoft runtime libraries when run in Windows environment.
include( InstallRequiredSystemLibraries )

# This must always be last!
include( CPack )
