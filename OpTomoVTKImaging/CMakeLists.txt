# CMakeList.txt: OpTomoVTKImaging için CMake projesi, kaynağı ekleyin ve tanımlayın
# projeye özgü mantık burada bulunur.
#

find_package(VTK REQUIRED)

include(${VTK_USE_FILE})
# Kaynağı bu projenin yürütülebilir dosyasına ekleyin.
add_library (OpTomoVTKImaging SHARED  "OpTomoVTKImaging.h" "OpTomoVTKImaging.cpp"  "Classes/Model.h" "Classes/Model.cpp" "Classes/VolumePipeline.h"   "Classes/VolumePipeline.cpp" "Classes/ReslicePipeline.h" "Classes/ReslicePipeline.cpp")

target_compile_definitions(OpTomoVTKImaging PRIVATE "MODEL_EXPORTS")
set_target_properties(OpTomoVTKImaging PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS YES)

target_include_directories(OpTomoVTKImaging PRIVATE ${VTK_INCLUDE_DIRS})
target_link_libraries(OpTomoVTKImaging PRIVATE ${VTK_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpTomoVTKImaging PROPERTY CXX_STANDARD 20)
endif()

# TODO: Gerekirse testleri ve yükleme hedeflerini ekleyin.
