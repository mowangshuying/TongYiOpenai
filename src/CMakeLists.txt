include_directories(../3rdparty/libhv)
add_executable(TongYiOpenAi main.cpp)
target_link_libraries(TongYiOpenAi ${HV_LIBRARIES})