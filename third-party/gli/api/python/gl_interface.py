"""
1 - ctypes
2 - cffi
3 - from distutils.core import setup, Extension
"""

import os
import ctypes

class PointGLI(ctypes.Structure):
    _fields_ = [("x", ctypes.c_float),
                ("y", ctypes.c_float),
                ("z", ctypes.c_float)]

class ColorGLI(ctypes.Structure):
    _fields_ = [("r", ctypes.c_float),
                ("g", ctypes.c_float),
                ("b", ctypes.c_float),
                ("a", ctypes.c_float)]

_gli_lib = None

def gli_loader():

    global _gli_lib

    try:
        _gli_lib = ctypes.CDLL(os.getcwd() + '/build/linux/x86_64/release/libgli-opengl.so')
    except OSError as e:
        print(f"Failed to load the library: {e}")
    else:
        if hasattr(_gli_lib, 'gli_backend_init'):
            print("python - gli_backend_init is available.")
        else:
            print("gli_backend_init is not available in the library.")

    _gli_lib.gli_backend_init.argtypes = [ctypes.c_void_p]
    _gli_lib.gli_backend_init.restype = None
    addr = ctypes.cast(_gli_lib.gli_backend_init, ctypes.c_void_p).value
    print(f"python - gli_backend_init: {addr:#x}")

    _gli_lib.gli_clear.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float]
    _gli_lib.gli_viewport.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32]
    _gli_lib.gli_rectangle.argtypes = [PointGLI, PointGLI, ColorGLI, ctypes.c_float]
    _gli_lib.gli_coordinate.argtypes = []
    _gli_lib.gli_circle.argtypes = [PointGLI, ctypes.c_float, PointGLI, ColorGLI, ctypes.c_int32, ctypes.c_float]

def gli_backend_init(base):

    if base:
        base = ctypes.cast(base, ctypes.c_void_p).value
    #print(f"python - base: {base:#x}")

    try:
        _gli_lib.gli_backend_init(base)
    except AttributeError:
        print("Function 'gli_backend_init' is not loaded properly.")
    except Exception as e:
        print(f"An error occurred: {e}")
    
    print("python - init done")

def gli_clear(r = 0, g = 0, b = 0, a = 1):
    _gli_lib.gli_clear(r, g, b, a)

def gli_backend_deinit():
    _gli_lib.gli_backend_deinit()

def gli_line(p1, p2, col = ColorGLI(1, 1, 1, 1), thickness = 1):
    _gli_lib.gli_line(p1, p2, col, thickness)

def gli_rectangle(p1, p2, col = ColorGLI(1, 1, 1, 1), thickness = 1):
    _gli_lib.gli_rectangle(p1, p2, col, thickness)

def gli_circle(center, radius, normal = PointGLI(0, 0, 1), col = ColorGLI(1, 1, 1, 1), segmentsNum = 36, thickness = 1):
    _gli_lib.gli_circle(center, radius, normal, col, segmentsNum, thickness)

def gli_viewport(x, y, w, h):
    _gli_lib.gli_viewport(x, y, w, h)

def gli_coordinate():
    _gli_lib.gli_coordinate()

gli_loader()