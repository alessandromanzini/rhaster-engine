include( FetchContent )

macro( fetch_sdl2 )
    if( APPLE )
        # SDL2
        fetchcontent_declare(
                SDL2
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG release-2.30.10
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE )
        fetchcontent_makeavailable( SDL2 )

        # SDL2 Image
        fetchcontent_declare(
                SDL2_image
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
                GIT_TAG release-2.8.3
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE )

        set( SDL2IMAGE_INSTALL OFF )
        fetchcontent_makeavailable( SDL2_image )

        # SDL2 ttf
        fetchcontent_declare(
                SDL2_ttf
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
                GIT_TAG release-2.22.0
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE )

        set( SDL2TTF_INSTALL OFF )
        fetchcontent_makeavailable( SDL2_ttf )

        # SDL2 Mixer
        fetchcontent_declare(
                SDL2_mixer
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
                GIT_TAG release-2.8.0
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE )

        set( SDL2MIXER_INSTALL OFF )
        set( SDL2MIXER_VENDORED ON )           # use bundled libraries when possible
        set( SDL2MIXER_OPUS OFF )              # disable Opus support
        fetchcontent_makeavailable( SDL2_mixer )

        # set all include directories
        target_include_directories(
                ${PROJECT_NAME} PUBLIC
                ${sdl2_SOURCE_DIR}/include
                ${sdl2_image_SOURCE_DIR}/include
                ${sdl2_ttf_SOURCE_DIR}
                ${sdl2_mixer_SOURCE_DIR}/include )

        # set libraries to link with
        target_link_libraries(
                ${PROJECT_NAME} PUBLIC
                SDL2::SDL2main
                SDL2::SDL2
                SDL2_image::SDL2_image
                SDL2_ttf::SDL2_ttf
                SDL2_mixer::SDL2_mixer
                $ENV{FREETYPE} )

    elseif( WIN32 )
        fetchcontent_declare(
                SDL2
                URL https://www.libsdl.org/release/SDL2-devel-2.30.10-VC.zip
                DOWNLOAD_NO_PROGRESS ON
                DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads )

        fetchcontent_getproperties( SDL2 )

        if( NOT SDL2_POPULATED )
            fetchcontent_populate( SDL2 )
            set( SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include )

            if( ${CMAKE_SIZEOF_VOID_P} MATCHES 8 )
                set( SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;" )
                set( SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll" )
            else()
                set( SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;" )
                set( SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll" )
            endif()
        endif()

        # add SDL2_image
        fetchcontent_declare(
                SDL2_image
                URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.3-VC.zip
                DOWNLOAD_NO_PROGRESS ON
                DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads )
        fetchcontent_getproperties( SDL2_image )

        if( NOT SDL2_image_POPULATED )
            fetchcontent_populate( SDL2_image )
            set( SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include )

            if( ${CMAKE_SIZEOF_VOID_P} MATCHES 8 )
                set( SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib" )
                set( SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll" )
            else()
                set( SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib" )
                set( SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll" )
            endif()
        endif()

        # add SDL2_ttf
        fetchcontent_declare(
                SDL2_ttf
                URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-VC.zip
                DOWNLOAD_NO_PROGRESS ON
                DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads )
        fetchcontent_getproperties( SDL2_ttf )

        if( NOT sdl2_ttf_POPULATED )
            fetchcontent_populate( SDL2_ttf )
            set( SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include )

            if( ${CMAKE_SIZEOF_VOID_P} MATCHES 8 )
                set( SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib )
                set( SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll" )
            else()
                set( SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib )
                set( SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll" )
            endif()
        endif()

        # add SDL2_mixer
        fetchcontent_declare(
                SDL2_mixer
                URL https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.8.0-VC.zip
                DOWNLOAD_NO_PROGRESS ON
                DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads )
        fetchcontent_getproperties( SDL2_mixer )

        if( NOT SDL2_mixer_POPULATED )
            fetchcontent_populate( SDL2_mixer )
            set( SDL2_MIXER_INCLUDE_DIR ${sdl2_mixer_SOURCE_DIR}/include )

            if( ${CMAKE_SIZEOF_VOID_P} MATCHES 8 )
                set( SDL2_MIXER_LIBRARIES "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.lib" )
                set( SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.dll" )
            else()
                set( SDL2_MIXER_LIBRARIES "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.lib" )
                set( SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.dll" )
            endif()
        endif()

        # set all include directories
        target_include_directories( ${PROJECT_NAME} PUBLIC
                                    ${VLD_INCLUDE_DIR}
                                    ${SDL2_INCLUDE_DIR}
                                    ${SDL2_IMAGE_INCLUDE_DIR}
                                    ${SDL2_TTF_INCLUDE_DIR}
                                    ${SDL2_MIXER_INCLUDE_DIR} )

        # set libraries to link with
        target_link_libraries( ${PROJECT_NAME} PUBLIC
                               ${SDL2_LIBRARIES}
                               ${SDL2_IMAGE_LIBRARIES}
                               ${SDL2_TTF_LIBRARIES}
                               ${SDL2_MIXER_LIBRARIES} )
    else()
        message( FATAL_ERROR "Unsupported platform for SDL2 fetchcontent" )
    endif()
endmacro()