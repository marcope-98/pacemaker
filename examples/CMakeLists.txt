add_executable(pacemaker-cli pacemaker_cli.cpp)
target_link_libraries(
  pacemaker-cli 
  PRIVATE 
    pacemaker::inca 
    pacemaker::io 
    pacemaker::timer
)
target_compile_features(pacemaker-cli PRIVATE cxx_std_17)