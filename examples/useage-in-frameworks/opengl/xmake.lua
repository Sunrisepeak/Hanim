add_rules("mode.debug", "mode.release")

set_languages("c++17")

add_requires("glfw 3.3.4")

add_includedirs("../../../")

-- link lib
if is_plat("linux") then
    add_links("pthread")
end

target("hanim-demo-opengl")
    set_kind("binary")
    add_packages("glfw")
    add_files("main.cpp")