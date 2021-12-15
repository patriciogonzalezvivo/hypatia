# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

file(GLOB ROOT_HEADER "${PROJECT_SOURCE_DIR}/include/hypatia/*.h")
file(GLOB ROOT_SOURCE "${PROJECT_SOURCE_DIR}/src/*.cpp")

file(GLOB COORD_HEADER "${PROJECT_SOURCE_DIR}/include/hypatia/coordinates/*.h")
file(GLOB COORD_SOURCE "${PROJECT_SOURCE_DIR}/src/coordinates/*.cpp")

file(GLOB MODEL_HEADER "${PROJECT_SOURCE_DIR}/include/hypatia/models/*.h")
file(GLOB MODEL_SOURCE "${PROJECT_SOURCE_DIR}/src/models/*.cpp")

file(GLOB PRIMI_HEADER "${PROJECT_SOURCE_DIR}/include/hypatia/primitives/*.h")
file(GLOB PRIMI_SOURCE "${PROJECT_SOURCE_DIR}/src/primitives/*.cpp")

set(INCLUDE_FOLDER "${PROJECT_SOURCE_DIR}/include")
set(INCLUDE_SWIG_FILE "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.i")

# Make an automatic library - will be static or dynamic based on user setting
add_library(hypatia ${ROOT_SOURCE} ${COORD_SOURCE} ${MODEL_SOURCE} ${PRIMI_SOURCE})

# We need this directory, and users of our library will need it too
target_include_directories(hypatia PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(   TREE "${PROJECT_SOURCE_DIR}/include" 
                PREFIX "Header Files" FILES ${ROOT_HEADER})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
    install(FILES ${ROOT_HEADER} DESTINATION include/hypatia)
    install(FILES ${COORD_HEADER} DESTINATION include/hypatia/coordinates)
    install(FILES ${MODEL_HEADER} DESTINATION include/hypatia/models)
    install(FILES ${PRIMI_HEADER} DESTINATION include/hypatia/primitives)


    # PYTHON BINDINGS 
    #
    if(POLICY CMP0078)
        cmake_policy(SET CMP0078 OLDif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME))
    endif(POLICY CMP0078)

    if(POLICY CMP0086)
        cmake_policy(SET CMP0086 OLD)
    endif(POLICY CMP0086)

    find_package(SWIG REQUIRED)
    include(${SWIG_USE_FILE})

    find_package(PythonLibs REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})

    if(NOT PYTHON_NUMPY_INCLUDE_DIR)
        execute_process(COMMAND python3 -c "import numpy; print(numpy.get_include())"
                        RESULT_VARIABLE PYTHON_NUMPY_PROCESS
                        OUTPUT_VARIABLE PYTHON_NUMPY_INCLUDE_DIR
                        OUTPUT_STRIP_TRAILING_WHITESPACE)

        if(PYTHON_NUMPY_PROCESS EQUAL 0)
            file(TO_CMAKE_PATH "${PYTHON_NUMPY_INCLUDE_DIR}" PYTHON_NUMPY_INCLUDE_CMAKE_PATH)
            set(PYTHON_NUMPY_INCLUDE_DIR ${PYTHON_NUMPY_INCLUDE_CMAKE_PATH} CACHE PATH "Numpy include directory")
        else()
            message(SEND_ERROR "Could not determine the NumPy include directory, verify that NumPy was installed correctly.")
        endif()
    endif()

    include_directories(${PYTHON_NUMPY_INCLUDE_DIR} ${INCLUDE_FOLDER})
    set_property(SOURCE ${INCLUDE_SWIG_FILE} PROPERTY CPLUSPLUS ON)

    if (CMAKE_VERSION VERSION_LESS "3.8")
        swig_add_module(${PROJECT_NAME} 
                        python 
                        ${INCLUDE_SWIG_FILE} ${ROOT_SOURCE} ${COORD_SOURCE} ${MODEL_SOURCE} ${PRIMI_SOURCE})
    else()
        swig_add_library(${PROJECT_NAME} 
                        LANGUAGE python
                        SOURCES ${INCLUDE_SWIG_FILE} ${ROOT_SOURCE} ${COORD_SOURCE} ${MODEL_SOURCE} ${PRIMI_SOURCE})
    endif()
    swig_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})

    # INSTALL PYTHON BINDINGS
    # Get the python site packages directory by invoking python
    execute_process(COMMAND python3 -c "import site; print(site.getsitepackages()[0])" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
    message("Installing at ${SITE_PACKAGES}")

    install(TARGETS _${PROJECT_NAME} DESTINATION ${PYTHON_SITE_PACKAGES})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.py DESTINATION ${PYTHON_SITE_PACKAGES})
endif()