add_rules("mode.debug", "mode.release")

set_languages("c++17")

add_requires("opencv 4.6")
add_requires("glfw 3.3.4")

add_includedirs("./")
add_includedirs("../../") -- root dir
add_includedirs("../../third-party")

-- link lib
if is_plat("linux") then
    add_links("pthread")
end

target("hanim-demo-anim")
    set_kind("binary")
    add_packages("opencv", "glfw")
    add_files("main.cpp")