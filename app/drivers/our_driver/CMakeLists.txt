# SPDX-License-Identifier: Apache-2.0
if (CONFIG_OUR_DRIVER)
    target_sources(app PRIVATE our_driver.c)
endif()
#add_subdirectory(drivers/our_driver)
