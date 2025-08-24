include( FetchContent )

macro( fetch_glm )
    fetchcontent_declare(
            glm
            URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    fetchcontent_makeavailable( glm )
    target_include_directories( ${PROJECT_NAME} PUBLIC ${glm_SOURCE_DIR} )
endmacro()
