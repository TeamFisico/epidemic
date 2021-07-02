############################# 1ST PART   ################################

set(HEADER_LIST_SEIR   "${epidemic_SOURCE_DIR}/include/epidemic1/seir.hpp"
                       "${epidemic_SOURCE_DIR}/include/epidemic1/seir_rks.hpp")

add_subdirectory(seir)


set(HEADER_LIST_SIM    "${epidemic_SOURCE_DIR}/include/simulation/random.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/randutils.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/parameters.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/position.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/location.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/rectangle.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/group.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/cluster.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/world.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/person.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/mobility_model.hpp"
                       "${epidemic_SOURCE_DIR}/include/simulation/simulation.hpp")

add_subdirectory(simulation)








