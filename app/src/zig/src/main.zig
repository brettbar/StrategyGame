const r = @cImport({
    @cDefine("_NO_CRT_STDIO_INLINE", "1");
    @cInclude("raylib.h");
});

const Type = enum {
    grid_panel,
};

const GridChild = struct {
    child: *Element,
    starting_column: u32,
    ending_column: u32,
    starting_row: u32,
    ending_row: u32,
};

const GridPanel = struct {
    num_cols: u32,
    num_rows: u32,
    grid: []GridChild,
};

const Element = union(Type) {
    grid_panel: GridPanel,
};

fn CreateMainMenu() GridPanel {
    GridPanel{
        .num_cols = 1,
        .num_rows = 6,
        .grid = {},
    };
}

const menu = CreateMainMenu();

export fn draw() void {
    r.DrawText("Hello from Zig!!!", 150, 150, 32, r.RED);
}

export fn add(a: i32, b: i32) i32 {
    return a + b;
}
