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
idf.py flash  (flashes entire project)
idf.py app-flash (flashes only the app)
idf.py bootloader-flash (flashes only the bootloader)
idf.py partition_table-flash (flashes only the partition table)
idf.py encrypted-flash (flashes encrypted project)
idf.py encrypted-app-flash (flash encrypted app only)

## Displaying Serial Output
idf.py monitor  (use ctrl ] to exit)

##CMakeLists.txt
in the application CMakeLists.txt for the source directory, it needs to include `list(APPEND EXTRA_COMPONENT_DIRS "../../../components/xxx/yyy")`

#additional information
The ESP-IDF build system does not allow whitespace characters in paths so any directories here must be without whitespace characters

