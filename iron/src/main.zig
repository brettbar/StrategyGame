const std = @import("std");
const rl = @import("raylib");

const DrawCallTag = enum(u32) {
    rect,
};
const DrawCall = extern struct {
    tag: DrawCallTag,
    rect: rl.Rectangle,
    bg: rl.Color,
};

const TypeTag = enum {
    grid,
};

const Grid = struct {
    transform: rl.Rectangle,
    bg: rl.Color,

    cols: u32,
    rows: u32,
    // children: []struct {
    //     col_start: u32,
    //     col_end: u32,
    //     row_start: u32,
    //     row_end: u32,
    //     child: *Element,
    // },

    pub fn to_draw(self: Grid) DrawCall {
        return DrawCall{
            .tag = .rect,
            .rect = self.transform,
            .bg = self.bg,
        };
    }
};

const Element = union(TypeTag) {
    grid: Grid,

    pub fn to_draw(self: Element) DrawCall {
        switch (self) {
            TypeTag.grid => self.grid.to_draw(),
        }
    }
};

fn create_main_menu() Element {
    var screen_width_f: f32 = @floatFromInt(rl.GetScreenWidth());
    var screen_height_f: f32 = @floatFromInt(rl.GetScreenHeight());

    const half_sw: f32 = screen_width_f / 2.0;
    const half_sh: f32 = screen_height_f / 2.0;

    return Element{
        .grid = Grid{
            .transform = rl.Rectangle{
                .x = half_sw - 150.0,
                .y = half_sh - 300.0,
                .width = 300.0,
                .height = 600.0,
            },
            .bg = rl.BLUE,
            .cols = 1,
            .rows = 6,
            // .children = []struct {},
        },
    };
}

export fn zig_draw() DrawCall {
    const menu = create_main_menu();

    return menu.grid.to_draw();
}
