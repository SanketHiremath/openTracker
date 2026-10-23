"""Shared dimensions and shape factories for the car-tracker enclosure.

All units mm. Origin: centre of the footprint; base floor underside at z=0.
Print-tolerance values (CLR_*, SNAP_*) assume a 0.4 mm nozzle, 0.2 mm layers,
PETG (or PLA) on a reasonably tuned printer; tune them first if a test fit
is too tight or loose.
"""
from __future__ import annotations

from cadgen import build123d as bd

# ---- contents (measured) -----------------------------------------------------
PCB_L, PCB_W, PCB_H = 99.5, 74.8, 16.2   # main perfboard; PCB_H = board + tallest part
GPS_L, GPS_W = 36.6, 25.9                # NEO-6M module (GPS_L along Y)
GPS_HOLE_INSET = 3.0                     # ASSUMED hole centre from module edges -- measure
MCU_L, MCU_W = 34.0, 18.0                # Pro Micro devkit (no holes: sits in a cradle)
JACK_D = 12.3                            # panel USB-C "barrel" connector body

# ---- layout clearances --------------------------------------------------------
CLR_X = 1.0            # item-to-wall at the short ends
CLR_Y = 3.0            # item-to-wall along the long sides (room for snap arms to flex)
GAP = 3.0              # GPS to MCU (Y)
EXTRA_L = 10.0         # extra X room between the main PCB and the GPS/MCU column
EXTRA_W = 23.0         # extra Y room (free strip on the -Y side) for the USB-C cable

# ---- structure ---------------------------------------------------------------
WALL = 2.4             # 6 x 0.4 mm perimeters
FLOOR = 2.4
LID_T = 2.4
CORNER_R = 8.0
EDGE_R = 2.0

# ---- print tolerances -------------------------------------------------------
CLR_LIP = 0.3          # lid lip-to-wall gap per side
CLR_HOLE = 0.2         # added to round holes (holes print undersize)
CLR_FIT = 0.3          # cradle slip fit per side

# ---- derived -----------------------------------------------------------------
COL_W = max(GPS_W, MCU_W)
IN_L = CLR_X + PCB_L + GAP + EXTRA_L + COL_W + CLR_X
IN_W = 2 * CLR_Y + max(PCB_W, GPS_L + GAP + MCU_L) + EXTRA_W
OUT_L = IN_L + 2 * WALL
OUT_W = IN_W + 2 * WALL
STANDOFF_H = 5.0
CAVITY_H = 32.0        # standoff 5 + PCB 16.2 + room for wires and the jack body
BASE_H = FLOOR + CAVITY_H
TOTAL_H = BASE_H + LID_T

# placement (centres)
PCB_C = (-IN_L / 2 + CLR_X + PCB_L / 2, IN_W / 2 - CLR_Y - PCB_W / 2)  # against +Y wall
COL_X = IN_L / 2 - CLR_X - COL_W / 2
GPS_C = (COL_X, IN_W / 2 - CLR_Y - GPS_L / 2)
# MCU centred in the free space between the GPS module and the -Y wall
_free_top = GPS_C[1] - GPS_L / 2
_free_bot = -IN_W / 2
MCU_C = (COL_X, (_free_top + _free_bot) / 2)

# ---- mounting ----------------------------------------------------------------
STANDOFF_OD = 6.5
PCB_HOLE_INSET = 3.0   # ASSUMED perfboard corner-hole centre from edges -- measure
SCREW_PILOT = 2.8 + CLR_HOLE   # M3 self-tapping, print-compensated (module holes are 3.2)
MOUNT_PTS = [(PCB_C[0] + sx * (PCB_L / 2 - PCB_HOLE_INSET), sy * (PCB_W / 2 - PCB_HOLE_INSET))
             for sx in (-1, 1) for sy in (-1, 1)]
MOUNT_PTS += [(GPS_C[0] + sx * (GPS_W / 2 - GPS_HOLE_INSET), GPS_C[1] + sy * (GPS_L / 2 - GPS_HOLE_INSET))
              for sx in (-1, 1) for sy in (-1, 1)]
CRADLE_H = 2.0
CRADLE_T = 1.6

# ---- snap fit ----------------------------------------------------------------
LIP_T = 1.6
LIP_D = 4.0
ARM_L = 12.0
ARM_W = 14.0
ARM_X = (-30.0, 30.0)
ARM_SLOT = 1.2
SNAP_P = 1.0
SNAP_Z0 = -(ARM_L - 2.0)
SNAP_RAMP = 1.7
SNAP_FLAT = 0.6
SNAP_RET = 1.0
ROOT_GUSSET = 0.0     # inward wedge stiffening the arm root (stress relief)
GUSSET_H = 3.0
POCKET_D = 1.2
POCKET_PLAY = 0.1
POCKET_W = ARM_W + 1.0

# ---- openings ----------------------------------------------------------------
# +X end wall, high so the connector body (behind the wall) clears GPS and MCU
JACK = dict(y=0.0, d=JACK_D + 0.3 + CLR_HOLE, z=FLOOR + 20.0)
VENT = dict(n=6, pitch=6.0, length=50.0, width=2.4)


def _rrect(length, width, radius, height):
    return bd.extrude(bd.RectangleRounded(length, width, radius), amount=height)


def _soften(part, z, radius):
    edges = [e for e in part.edges() if abs(e.center().Z - z) < 1e-3]
    return bd.fillet(edges, radius)


def _snap_bump(sy, x):
    """Hook profile in the arm's (outward, z) plane, extruded across the arm."""
    y0 = sy * (IN_W / 2 - CLR_LIP)
    z = SNAP_Z0
    pts = [(0, z), (SNAP_P, z + SNAP_RAMP), (SNAP_P, z + SNAP_RAMP + SNAP_FLAT),
           (0, z + SNAP_RAMP + SNAP_FLAT + SNAP_RET)]
    pts = [(sy * p, zz) for p, zz in pts]
    face = bd.Pos(0, 0, 0) * bd.Plane.YZ.offset(0) * bd.Polygon(*pts, align=None)
    prism = bd.extrude(face, amount=ARM_W / 2, both=True)
    return bd.Pos(x, y0, 0) * prism


def _root_gusset(sy, x):
    y_in = sy * (IN_W / 2 - CLR_LIP - LIP_T)
    pts = [(y_in, -GUSSET_H), (y_in - sy * ROOT_GUSSET, 0.0), (y_in, 0.0)]
    face = bd.Plane.YZ * bd.Polygon(*pts, align=None)
    return bd.Pos(x, 0, 0) * bd.extrude(face, amount=ARM_W / 2, both=True)


def base_shape():
    body = _rrect(OUT_L, OUT_W, CORNER_R, BASE_H)
    body = _soften(body, 0.0, EDGE_R)
    cavity = bd.Pos(0, 0, FLOOR) * _rrect(IN_L, IN_W, CORNER_R - WALL, CAVITY_H + 1)
    body = body - cavity

    for x, y in MOUNT_PTS:
        post = bd.Pos(x, y, FLOOR) * bd.Cylinder(STANDOFF_OD / 2, STANDOFF_H,
                                                 align=(bd.Align.CENTER, bd.Align.CENTER, bd.Align.MIN))
        body = body + post
        hole = bd.Pos(x, y, FLOOR + 0.8) * bd.Cylinder(SCREW_PILOT / 2, STANDOFF_H + 1,
                                                       align=(bd.Align.CENTER, bd.Align.CENTER, bd.Align.MIN))
        body = body - hole

    # snap windows: blind recesses in the inner long walls
    # the wall surface sits CLR_LIP outboard of the arm face; the retention slope
    # meets the pocket's top ledge there
    hook_top = SNAP_Z0 + SNAP_RAMP + SNAP_FLAT + SNAP_RET
    ledge = BASE_H + hook_top - SNAP_RET * CLR_LIP / SNAP_P + POCKET_PLAY
    pocket_h = 4.2
    for sy in (-1, 1):
        for x in ARM_X:
            y_in = sy * IN_W / 2
            box = bd.Box(POCKET_W, POCKET_D, pocket_h, align=(bd.Align.CENTER, bd.Align.CENTER, bd.Align.MAX))
            body = body - bd.Pos(x, y_in + sy * POCKET_D / 2, ledge) * box

    # USB-C panel connector hole
    hole = bd.Rot(0, 90, 0) * bd.Cylinder(JACK["d"] / 2, WALL + 2)
    body = body - bd.Pos(OUT_L / 2 - WALL / 2, JACK["y"], JACK["z"]) * hole

    # MCU cradle: two low side rails + end stop
    inner = MCU_W + 2 * CLR_FIT
    for sx in (-1, 1):
        rail = bd.Box(CRADLE_T, MCU_L, CRADLE_H, align=(bd.Align.CENTER, bd.Align.CENTER, bd.Align.MIN))
        body = body + bd.Pos(MCU_C[0] + sx * (inner + CRADLE_T) / 2, MCU_C[1], FLOOR) * rail
    return body


def lid_shape():
    """Lid in assembly orientation: plate z 0..LID_T, lip/arms hang below z=0."""
    plate = _rrect(OUT_L, OUT_W, CORNER_R, LID_T)
    plate = _soften(plate, LID_T, EDGE_R)

    if VENT:
        for i in range(VENT["n"]):
            y = (i - (VENT["n"] - 1) / 2) * VENT["pitch"]
            slot = bd.extrude(bd.SlotOverall(VENT["length"], VENT["width"]), amount=LID_T + 2)
            plate = plate - bd.Pos(0, y, -1) * slot

    lo_l, lo_w = IN_L - 2 * CLR_LIP, IN_W - 2 * CLR_LIP
    r = CORNER_R - WALL - CLR_LIP
    ring = _rrect(lo_l, lo_w, r, LIP_D) - _rrect(lo_l - 2 * LIP_T, lo_w - 2 * LIP_T, max(r - LIP_T, 1), LIP_D)
    lid = plate + bd.Pos(0, 0, -LIP_D) * ring

    for sy in (-1, 1):
        y_out = sy * lo_w / 2
        for x in ARM_X:
            arm = bd.Box(ARM_W, LIP_T, ARM_L, align=(bd.Align.CENTER, bd.Align.CENTER, bd.Align.MAX))
            lid = lid + bd.Pos(x, y_out - sy * LIP_T / 2, 0.01) * arm
            for sx in (-1, 1):
                dx = x + sx * (ARM_W / 2 + ARM_SLOT / 2)
                slot = bd.Box(ARM_SLOT, LIP_T + 0.2, LIP_D, align=(bd.Align.CENTER, bd.Align.CENTER, bd.Align.MAX))
                lid = lid - bd.Pos(dx, y_out - sy * LIP_T / 2, 0) * slot
            lid = lid + _snap_bump(sy, x)
            if ROOT_GUSSET:
                lid = lid + _root_gusset(sy, x)
    return lid
