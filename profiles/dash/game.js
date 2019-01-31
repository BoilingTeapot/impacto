root.LayerCount = 100;
root.GameFeatures = GameFeature.Sc3VirtualMachine | GameFeature.Renderer2D | GameFeature.Scene3D | GameFeature.Input | GameFeature.Audio;
root.DesignWidth = 1280;
root.DesignHeight = 720;

root.Vm = {
    StartScript: 4,
    StartScriptBuffer: 0,
    GameInstructionSet: InstructionSet.RNE,
    UseReturnIds: true
};

include('dash/vfs.js');
include('dash/sprites.js');
include('common/animation.js');
include('dash/charset.js');
//include('dash/font.js');
include('dash/font-lb.js');
include('dash/dialogue.js');
include('dash/hud/saveicon.js');
include('dash/hud/loadingdisplay.js');
include('dash/hud/datedisplay.js');
include('dash/hud/titlemenu.js');
include('dash/hud/mainmenu.js');
include('dash/hud/sysmesboxdisplay.js');
include('dash/scene3d/scene3d.js');