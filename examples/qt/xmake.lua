add_rules("mode.debug", "mode.release")

set_languages("c++17")

add_rules("qt.widgetapp")

add_includedirs("../../")

if is_host("linux") then
    add_links("pthread")
end

target("hanim-demo-qt")
    add_files("main.cpp")