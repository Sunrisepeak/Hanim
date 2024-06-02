add_rules("mode.debug", "mode.release")

add_requires("glfw", "glew", "glad")

add_includedirs("../../")

add_files("gli_opengl.c")

target("gli-opengl")
    set_kind("binary")
    add_files("main.c")
    add_packages("glfw", "glew", "glad")