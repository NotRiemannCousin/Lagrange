# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create the library
add_library(lagrange-essentials
        src/Util.cpp
        src/NumericalMethods/Newton.cpp
        src/NumericalMethods/Bisection.cpp
        src/NumericalMethods/FixedPoint.cpp
        src/NumericalMethods/Secant.cpp
        src/Function.cpp)

# Specify C++ standard for the library
target_compile_features(lagrange-essentials PUBLIC cxx_std_23)

# Create the executable
add_executable(Lagrange main.cpp)

# Specify C++ standard for the executable
target_compile_features(Lagrange PRIVATE cxx_std_23)

# Compiler flags
target_compile_options(Lagrange PRIVATE -fconcepts -fconcepts-diagnostics-depth=200)
target_compile_options(lagrange-essentials PRIVATE -fconcepts -fconcepts-diagnostics-depth=200)

# Link the library to the executable
target_link_libraries(Lagrange PRIVATE lagrange-essentials stdc++exp)
