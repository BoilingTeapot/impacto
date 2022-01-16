root.TitleMenu = {
    Type: TitleMenuType.CCLCC,
    DrawType: DrawComponentType.SystemMenu,
    PressToStartX: 823,
    PressToStartY: 749,
    PressToStartAnimDurationIn: 0.7,
    PressToStartAnimDurationOut: 0.7,
    PressToStartSprite: "TitleMenuPressToStart",
    BackgroundSprite: "TitleMenuBackground",
    MainBackgroundSprite: "MainMenuBackground",
    CopyrightTextSprite: "CopyrightText",
    CopyrightTextX: 566,
    CopyrightTextY: 955,
    MenuSprite: "TitleMenuMenu",
    MenuX: 27,
    MenuY: 26,
    ItemHighlightSprite: "TitleMenuItemHighlight",
    ItemHighlightOffsetX: 174,
    ItemHighlightOffsetY: 7,
    ItemHighlightPointerSprite: "TitleMenuPointerItemHighlight",
    ItemHighlightPointerY: 89,
    ItemPadding: 56,
    ItemYBase: 335,
    MenuEntriesSprites: [],
    MenuEntriesHighlightedSprites: [],
    MenuEntriesNum: 5
};

for (var i = 0; i < 5; i++) {
    root.Sprites["TitleMenuEntry" + i] = {
        Sheet: "TitleChip",
        Bounds: {
            X: 33,
            Y: 288 + i * root.TitleMenu.ItemPadding,
            Width: 223,
            Height: 37
        }
    };
    root.TitleMenu.MenuEntriesSprites.push("TitleMenuEntry" + i);
};

for (var i = 0; i < 5; i++) {
    root.Sprites["TitleMenuEntry" + i] = {
        Sheet: "TitleChip",
        Bounds: {
            X: 33,
            Y: 288 + i * root.TitleMenu.ItemPadding,
            Width: 223,
            Height: 37
        }
    };
    root.TitleMenu.MenuEntriesHighlightedSprites.push("TitleMenuEntry" + i);
};

root.Sprites["TitleMenuItemHighlight"] = {
    Sheet: "TitleChip",
    Bounds: { X: 896, Y: 64, Width: 638, Height: 50 },
};

root.Sprites["TitleMenuPointerItemHighlight"] = {
    Sheet: "TitleChip",
    Bounds: { X: 805, Y: 78, Width: 56, Height: 36 },
};

root.Sprites["TitleMenuBackground"] = {
    Sheet: "Title",
    Bounds: { X: 0, Y: 0, Width: 1920, Height: 1080 },
};

root.Sprites["MainMenuBackground"] = {
    Sheet: "Title",
    Bounds: { X: 0, Y: 1080, Width: 1920, Height: 1080 },
};

root.Sprites["TitleMenuPressToStart"] = {
    Sheet: "TitleChip",
    Bounds: { X: 583, Y: 75, Width: 198, Height: 169 },
};

root.Sprites["CopyrightText"] = {
    Sheet: "TitleChip",
    Bounds: { X: 582, Y: 9, Width: 785, Height: 21 },
};

root.Sprites["TitleMenuMenu"] = {
    Sheet: "TitleChip",
    Bounds: { X: 0, Y: 0, Width: 544, Height: 248 },
};
