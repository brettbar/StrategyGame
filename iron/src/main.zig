const std = @import("std");
const rl = @import("raylib");

const Transform = extern struct {
    x: u32,
    y: u32,
    w: u32,
    h: u32,
    c: rl.Color,
};

const TypeTag = enum {
    grid,
};

const Grid = struct {
    cols: u32,
    rows: u32,
    children: []struct {
        col_start: u32,
        col_end: u32,
        row_start: u32,
        row_end: u32,
        child: *Element,
    },

    pub fn draw_grid() void {}
};

const Element = union(TypeTag) {
    grid: Grid,
};

export fn zig_draw() Transform {
    return Transform{
        .x = 0,
        .y = 0,
        .w = 200,
        .h = 200,
        .c = rl.BLUE,
    };
}
