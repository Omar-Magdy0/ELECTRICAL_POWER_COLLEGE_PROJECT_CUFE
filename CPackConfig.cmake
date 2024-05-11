# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "/media/renderer/59265a2b-d6f5-4b6a-ad90-6c1b1b9d04ac/Programming/ELECTRICAL_POWER_CUFE;/media/renderer/59265a2b-d6f5-4b6a-ad90-6c1b1b9d04ac/Programming/ELECTRICAL_POWER_CUFE")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_COMPONENTS_ALL "dependencies;libraries")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-3.25/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "energy_monitor built using CMake")
set(CPACK_DMG_SLA_USE_RESOURCE_FILE_LICENSE "ON")
set(CPACK_GENERATOR "ZIP")
set(CPACK_INSTALL_CMAKE_PROJECTS "/media/renderer/59265a2b-d6f5-4b6a-ad90-6c1b1b9d04ac/Programming/ELECTRICAL_POWER_CUFE;energy_monitor;ALL;/")
set(CPACK_INSTALL_PREFIX "/usr/local")
set(CPACK_MODULE_PATH "/media/renderer/59265a2b-d6f5-4b6a-ad90-6c1b1b9d04ac/Programming/ELECTRICAL_POWER_CUFE/bin/_deps/pdfhummus-src/cmake")
set(CPACK_NSIS_DISPLAY_NAME "energy_monitor 4.6")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "energy_monitor 4.6")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OBJCOPY_EXECUTABLE "/usr/bin/objcopy")
set(CPACK_OBJDUMP_EXECUTABLE "/usr/bin/objdump")
set(CPACK_OUTPUT_CONFIG_FILE "/media/renderer/59265a2b-d6f5-4b6a-ad90-6c1b1b9d04ac/Programming/ELECTRICAL_POWER_CUFE/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-3.25/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "energy_monitor built using CMake")
set(CPACK_PACKAGE_FILE_NAME "energy_monitor-4.6-Linux")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "energy_monitor 4.6")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "energy_monitor 4.6")
set(CPACK_PACKAGE_NAME "energy_monitor")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "4.6")
set(CPACK_PACKAGE_VERSION_MAJOR "4")
set(CPACK_PACKAGE_VERSION_MINOR "6")
set(CPACK_READELF_EXECUTABLE "/usr/bin/readelf")
set(CPACK_RESOURCE_FILE_LICENSE "/media/renderer/59265a2b-d6f5-4b6a-ad90-6c1b1b9d04ac/Programming/ELECTRICAL_POWER_CUFE/bin/_deps/pdfhummus-src/LICENSE")
set(CPACK_RESOURCE_FILE_README "/usr/share/cmake-3.25/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake-3.25/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/media/renderer/59265a2b-d6f5-4b6a-ad90-6c1b1b9d04ac/Programming/ELECTRICAL_POWER_CUFE/CPackSourceConfig.cmake")
set(CPACK_SYSTEM_NAME "Linux")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "Linux")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/media/renderer/59265a2b-d6f5-4b6a-ad90-6c1b1b9d04ac/Programming/ELECTRICAL_POWER_CUFE/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
