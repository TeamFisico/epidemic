

set(HEADER_LIST1 "${epidemic_SOURCE_DIR}/include/epidemic1/SEIR.hpp"
                 "${epidemic_SOURCE_DIR}/include/epidemic1/SEIRode.hpp")

add_subdirectory(epidemic1)

####################################################################################
############                    2nd part                          ##################
####################################################################################


set(HEADER_LIST2  "${epidemic_SOURCE_DIR}/include/epidemic2/position.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic2/person.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic2/simulation.hpp")

add_subdirectory(epidemic2)


set(HEADER_LIST3  "${epidemic_SOURCE_DIR}/include/epidemic3/location.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/group.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/cluster.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/area.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/person.hpp"
                  "${epidemic_SOURCE_DIR}/include/epidemic3/mobility_model.hpp")

add_subdirectory(epidemic3)
# IDE will put headers in a nice way





