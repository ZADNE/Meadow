add_subdirectory(external_shaders)
add_subdirectory(shaders)

target_sources(Meadow PRIVATE
    DirtDrawer.hpp              DirtDrawer.cpp
)
