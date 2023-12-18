add_subdirectory(external_shaders)
add_subdirectory(shaders)

target_sources(Meadow PRIVATE
    ViewFrustumDrawer.hpp       ViewFrustumDrawer.cpp
)
