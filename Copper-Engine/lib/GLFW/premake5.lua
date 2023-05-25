project "GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir("Build/" .. outputDir)
    objdir("BuildInt/" .. outputDir)

    files {

        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/glfw_config.h",

        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/vulkan.c",
        "src/window.c"

    }

    filter "system:windows"
        systemversion "latest"

        files {

            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_thread.c",
            "src/win32_window.c",
            "src/wgl_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c"

        }

        defines { "_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS" }
    
    filter "system:linux"
        files {

            "src/x11_init.c",
            "src/x11_monitor.c",
            "src/x11_window.c",
            "src/xkb_unicode.c",
            "src/posix_thread.c",
            "src/posix_time.c",
            "src/linux_joystick.c",
            "src/egl_context.c",
            "src/osmesa_context.c",
            "src/glx_context.c"

        }

        linkoptions {

            "-lX11"

        }

        defines { "_GLFW_X11", "_CRT_SECURE_NO_WARNINGS" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"