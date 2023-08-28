add_rules("mode.debug", "mode.release")

set_languages("c++17")

add_includedirs("./")
add_includedirs("../../") -- root dir
add_includedirs("../../third-party")

-- link lib
if is_plat("windows") then
    add_links("opengl32")
elseif is_plat("linux") then
    add_includedirs("/usr/include/opencv4")
    add_links("GL")
    add_links("pthread")
    add_links("opencv_core")
    add_links("opencv_highgui")
    add_links("opencv_imgproc", "opencv_videoio", "opencv_imgcodecs")
end

add_links("glfw")

target("hanim-demo-anim")
    set_kind("binary")
    add_files("main.cpp")