root.Fonts = {
    "Default": {
        Type: FontType.LB,
        ForegroundSheet: "FontLBForeground",
        OutlineSheet: "FontLBOutline",
        Columns: 64,
        Rows: 125,
        Widths: "games/cc/font-lb/widths.bin",
        DesignColWidth: 48,
        OutlineOffset: { X: -4, Y: -4 },
        LineSpacing: 14,
    }
};

delete root.SpriteSheets["Font"];

root.SpriteSheets["FontLBForeground"] = {
    Path: "games/cc/font-lb/foreground.png",
    DesignWidth: 3072,
    DesignHeight: 6000
};

root.SpriteSheets["FontLBOutline"] = {
    Path: "games/cc/font-lb/outline.png",
    DesignWidth: 3648,
    DesignHeight: 7128
};