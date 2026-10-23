from cadgen import build123d as bd
from cadgen import step
from parts import BASE_H, base_shape, lid_shape


@step(out="../STEP/assembly.step")
def assembly():
    base = base_shape()
    base.label = "base"
    lid = bd.Pos(0, 0, BASE_H) * lid_shape()
    lid.label = "lid"
    return bd.Compound(children=[base, lid], label="enclosure")


if __name__ == "__main__":
    assembly()
