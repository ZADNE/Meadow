add_subdirectory(shaders)

target_sources(Meadow PRIVATE
    BladeDrawer.hpp             BladeDrawer.cpp
    BladeSB.hpp                 
    DirtDrawer.hpp              DirtDrawer.cpp
    TerrainDrawer.hpp           TerrainDrawer.cpp
    TerrainUB.hpp               
)
