add_rules("mode.debug", "mode.release")

add_includedirs("../third-party")

if is_plat("linux") then
    add_includedirs("/usr/include/opencv4")
end

add_links("opencv_core")
add_links("opencv_highgui")
add_links("opencv_imgproc", "opencv_videoio")
add_links("glfw")

add_defines("HANIM_ENABLE_XRECORDER_OPENGL")

-- sub-project
add_subdirs("imgui")
add_subdirs("opengl")
add_subdirs("qt")