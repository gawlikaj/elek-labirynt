# Notes for Working with the ESP-IDF build system

## Creating a New Project

idf.py create-project --path <project path> <path name>

## Setting target
idf.py set-target esp32

## Creating a New Component
idf.py -C <component path> create-component <component name>

## Building a Project
idf.py build   (builds everything)
idf.py app     (builds only the app)
idf.py bootloader (builds only the bootloader)
idf.py partition-table (builds only the partition table)
idf.py clean (delete build output files from the build directory)
idf.py fullclean (delete the entire build directory contents)

## Flashing a Project
note:  before the appropriate flash command, one can enter -p port where port is the port to use for flashing
idf.py flash  (flashes entire project)
idf.py app-flash (flashes only the app)
idf.py bootloader-flash (flashes only the bootloader)
idf.py partition_table-flash (flashes only the partition table)
idf.py encrypted-flash (flashes encrypted project)
idf.py encrypted-app-flash (flash encrypted app only)

## Displaying Serial Output
note:  before the appropriate flash command, one can enter -p port where port is the port to use for monitoring
idf.py monitor  (use ctrl ] to exit)

##CMakeLists.txt
in the application CMakeLists.txt for the source directory, it needs to include `list(APPEND CUSTOM_COMPONENTS "xxx/yyy")` where "xxx" is an indicator of "freertos" or "baremetal" and "yyy" is the component name.  Also after all custom components have been added, this line needs to be included prior to the inclusion of project.cmake `include("../../custom_components.cmake")` this will convert the custom components to the correct paths for use in the build process.

##Unit Testing
Unit tests can be created for each component by adding a "test" directory inside including the following:
  `idf_component_register(SRC_DIRS .
                       PRIV_REQUIRES cmock test_utils xxx)`
  where "xxx" represents all dependency components required for the functionality of the component
  the source file includes all test cases one wishes to use (see unity for information on how to implement test cases)
  to run the tests, one needs to navigate to `esp/esp-idf/tools/unit-test-app/` and update the CMakeLists.txt to include the appropriate paths for the components to be tested.  After that one runs `idf.py -T xxx build` where xxx is the component to be tested followed by flashing and monitoring as appropriate.  For more interactive testing, a separate application is advisable.

#additional information
The ESP-IDF build system does not allow whitespace characters in paths so any directories here must be without whitespace characters

