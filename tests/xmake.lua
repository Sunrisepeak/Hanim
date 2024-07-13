add_rules("mode.debug", "mode.release")

add_requires("glfw", "glm", "opencv")

includes("../third-party/gli")

add_includedirs("../")
add_includedirs("../third-party/gli")
add_includedirs("../third-party/honly")

if is_host("linux") then
    add_includedirs("/usr/include/opencv4")
end

add_links("opencv_core")
add_links("opencv_highgui")
add_links("opencv_imgproc", "opencv_videoio", "opencv_imgcodecs")

target("tests")
    set_kind("binary")
    add_deps("gli-opengl")
    add_files("main.cpp")
    add_packages("glm", "glfw", "opencv") -- TODO: remove glfw function from scene