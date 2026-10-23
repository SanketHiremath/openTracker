import sys; sys.path.insert(0, "src")
from cadgen import build123d as bd
from parts import *
b, l = base_shape(), lid_shape()
for n, s in (("base", b), ("lid", l)):
    bb = s.bounding_box()
    print(n, "valid", s.is_valid, "solids", len(s.solids()), "vol %.0f" % s.volume,
          "size %.1f x %.1f x %.1f" % (bb.size.X, bb.size.Y, bb.size.Z), "zmin %.2f zmax %.2f" % (bb.min.Z, bb.max.Z))
lid_up = bd.Pos(0, 0, BASE_H) * l
print("interference vol", (b & lid_up).volume)
print("inner %.1f x %.1f, outer %.1f x %.1f x %.1f" % (IN_L, IN_W, OUT_L, OUT_W, TOTAL_H))
# dummy contents
def blk(c, l, w, z0, h):
    return bd.Pos(c[0], c[1], z0) * bd.Box(l, w, h, align=(bd.Align.CENTER, bd.Align.CENTER, bd.Align.MIN))
pcb = blk(PCB_C, PCB_L, PCB_W, FLOOR + STANDOFF_H, PCB_H)
gps = blk(GPS_C, GPS_W, GPS_L, FLOOR + STANDOFF_H, 8)
mcu = blk(MCU_C, MCU_W, MCU_L, FLOOR, 8)
jack = bd.Pos(OUT_L/2 - WALL - 7.5, 0, JACK["z"]) * (bd.Rot(0, 90, 0) * bd.Cylinder(6.3, 15))
for n, c in (("pcb", pcb), ("gps", gps), ("mcu", mcu), ("jack body 12.6x15", jack)):
    print(n, "vs base %.2f" % (b & c).volume, "vs lid %.2f" % (lid_up & c).volume)
