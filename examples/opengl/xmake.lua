set_project("OpenGLCircle")

set_languages("c++17")

add_includedirs("../../")

if is_plat("windows") then
    add_links("opengl32")
elseif is_plat("linux") then
    add_links("GL")
    add_links("pthread")
end

add_links("glfw")

target("hanim-demo-opengl")
    set_kind("binary")
    add_files("main.cpp")