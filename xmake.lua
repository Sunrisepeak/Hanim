add_rules("mode.debug", "mode.release")

add_requires("glfw", "glm")
add_requires("opencv", {system = true})
add_packages("glfw", "glm", "opencv")

add_repositories("sunrisepeak-projects git@github.com:Sunrisepeak/sunrisepeak-projects.git")
add_requires("gl-interface")
add_packages("gl-interface")

add_includedirs(".")

includes("tests")
includes("examples/hello-animation")
includes("examples/hi-animation")
includes("examples/pixels/hello-pixels")