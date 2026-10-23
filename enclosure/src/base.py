from cadgen import step, stl
from parts import base_shape


@step(out="../STEP/base.step")
@stl(out="../STL/base.stl")
def base():
    shape = base_shape()
    shape.label = "base"
    return shape


if __name__ == "__main__":
    base()
