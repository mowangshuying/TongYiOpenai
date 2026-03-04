add_executable(More More.cpp)
target_link_libraries(More ${HV_LIBRARIES})

add_executable(TongYiChat TongYiChat.cpp)
target_link_libraries(TongYiChat ${HV_LIBRARIES})

add_executable(Translate Translate.cpp)
target_link_libraries(Translate ${HV_LIBRARIES})

add_executable(ImageGeneration ImageGeneration.cpp)
target_link_libraries(ImageGeneration ${HV_LIBRARIES})



