cmake_minimum_required(VERSION 3.10.0)
project(paqueteCnC VERSION 0.1.0)
add_library(paqueteCnCLibrary
  src/packet.cpp
  src/packet_operations.cpp
  src/packet_builder.cpp)

add_executable(paqueteCnC src/main.cpp)
target_link_libraries(paqueteCnC paqueteCnCLibrary)
target_compile_features(paqueteCnC PRIVATE cxx_std_20)
target_compile_features(paqueteCnCLibrary PRIVATE cxx_std_20)
