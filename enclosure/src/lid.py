from cadgen import build123d as bd
from cadgen import step, stl
from parts import LID_T, lid_shape


@step(out="../STEP/lid.step")
@stl(out="../STL/lid.stl")
def lid():
    # Print orientation: top face on the bed, lip and snap arms pointing up.
    shape = bd.Pos(0, 0, LID_T) * (bd.Rot(180, 0, 0) * lid_shape())
    shape.label = "lid"
    return shape


if __name__ == "__main__":
    lid()
