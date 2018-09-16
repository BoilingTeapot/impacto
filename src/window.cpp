#include <GL/glew.h>
#include <SDL_opengl.h>

#include "window.h"
#include "log.h"

namespace Impacto {

static bool IsInit = false;
int g_WindowWidth;
int g_WindowHeight;
SDL_Window* g_SDLWindow;
SDL_GLContext g_GLContext;

void WindowInit() {
  assert(IsInit == false);
  ImpLog(LL_Info, LC_General, "Creating window\n");
  IsInit = true;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
    ImpLog(LL_Fatal, LC_General, "SDL initialisation failed: %s\n",
           SDL_GetError());
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // TODO do we need a stencil buffer?
  // SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

  // TODO DPI aware

  int contextFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
  // TODO this should really be a runtime decision
#ifdef IMPACTO_GL_DEBUG
  contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags);

  g_SDLWindow = SDL_CreateWindow("impacto", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, 1280, 720,
                                 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if (g_SDLWindow == NULL) {
    ImpLog(LL_Fatal, LC_General, "SDL window creation failed: %s\n",
           SDL_GetError());
    exit(1);
  }

  SDL_GetWindowSize(g_SDLWindow, &g_WindowWidth, &g_WindowHeight);
  ImpLog(LL_Debug, LC_General, "Window size (screen coords): %d x %d\n",
         g_WindowWidth, g_WindowHeight);

  g_GLContext = SDL_GL_CreateContext(g_SDLWindow);
  if (g_GLContext == NULL) {
    ImpLog(LL_Fatal, LC_General, "OpenGL context creation failed: %s\n",
           SDL_GetError());
    exit(1);
  }

  SDL_GL_GetDrawableSize(g_SDLWindow, &g_WindowWidth, &g_WindowHeight);
  ImpLog(LL_Debug, LC_General, "Drawable size (pixels): %d x %d\n",
         g_WindowWidth, g_WindowHeight);

  glewExperimental = GL_TRUE;
  GLenum glewErr = glewInit();
  if (glewErr != GLEW_OK) {
    ImpLog(LL_Fatal, LC_General, "GLEW initialisation failed: %s\n",
           glewGetErrorString(glewErr));
    exit(1);
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef IMPACTO_GL_DEBUG
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallbackARB(&LogGLMessageCallback, NULL);
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
                             GL_TRUE);
  } else {
    ImpLog(LL_Error, LC_GL, "Could not get debug context\n");
  }
#endif

  // Vsync
  SDL_GL_SetSwapInterval(1);
}

}  // namespace Impacto