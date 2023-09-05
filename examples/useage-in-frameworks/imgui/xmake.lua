add_rules("mode.debug", "mode.release")

add_requires("imgui", {configs = {glfw_opengl3 = true}})

set_languages("c++17")

add_includedirs(".")
add_includedirs("../../../")

if is_host("linux") then
    add_links("pthread")
end

target("hanim-demo-imgui")
    set_kind("binary")
    add_files("main.cpp")
    add_packages("imgui")