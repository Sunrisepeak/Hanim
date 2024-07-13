target("gli-opengl-c")
    set_kind("binary")
    add_deps("gli-opengl")
    add_files("opengl/main.c")
    add_packages("glfw", "glad", "glm")

-- only compile test
target("gli-opengl-static-c")
    set_kind("binary")
    add_deps("gli-opengl.static")
    --add_files("../backends/gli_opengl.cpp")
    add_files("opengl/main.c")
    add_packages("glfw", "glad", "glm")

target("gli-opengl-py")
    set_kind("phony")

    on_run(function (target)
        os.exec("python3 examples/opengl/main.py")
    end)