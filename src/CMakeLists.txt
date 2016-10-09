SET(MAIN_SOURCES
  main.cpp
  Atom.cpp
  Matrix.cpp
  Simulation.cpp
  Vec.cpp
  )

SET(MAIN_HEADERS
    common.h)

ADD_EXECUTABLE(${TARGET_MAIN} ${MAIN_SOURCES} ${MAIN_HEADERS})
