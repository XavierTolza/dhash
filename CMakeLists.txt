cmake_minimum_required(VERSION 3.12)
project(DHashExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
# Spécifier la version minimale d'OpenCV requise
find_package(OpenCV 4 REQUIRED)

# Spécifier les fichiers source de votre application
set(SOURCES main.cpp)

# Créer l'exécutable
add_executable(DHashExample ${SOURCES})

# Inclure les fichiers d'en-tête d'OpenCV
target_include_directories(DHashExample PRIVATE ${OpenCV_INCLUDE_DIRS})

# Lier les bibliothèques OpenCV
target_link_libraries(DHashExample PRIVATE ${OpenCV_LIBS})
