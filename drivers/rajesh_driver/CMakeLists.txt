if(CONFIG_RAJESH_DRIVER)
    target_sources(app PRIVATE rajesh_driver.c)
    target_include_directories(app PRIVATE include)
endif()
