macro(reload_resources TARGET_NAME RESOURCES_SRC_FOLDER RESOURCES_DST_FOLDER)
    # set artifacts reloading
    set(RELOAD_COMMAND "reload_resources")

    # gather all artifacts from source directories
    file(GLOB_RECURSE MY_RESOURCES "${RESOURCES_SRC_FOLDER}/*")

    # define a custom command to copy artifacts
    # this command depends on the files in RESOURCES_SRC_FOLDER
    add_custom_command( OUTPUT ${RESOURCES_DST_FOLDER}
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${RESOURCES_SRC_FOLDER} ${RESOURCES_DST_FOLDER}
            DEPENDS ${MY_RESOURCES}
            COMMENT "Reloading resources." )

    add_custom_target(${RELOAD_COMMAND} ALL
            DEPENDS ${RESOURCES_DST_FOLDER} )

    # ensure your main project depends on the custom command
    add_dependencies(${PROJECT_NAME} ${RELOAD_COMMAND})

    message( STATUS "Artifacts reloading added as '${PROJECT_NAME}' dependency." )
endmacro()