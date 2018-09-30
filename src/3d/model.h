#pragma once

#include "../impacto.h"
#include "../texture/texture.h"
#include "transform.h"

#include <glm/glm.hpp>

#include "../../vendor/include/flat_hash_map.hpp"

namespace Impacto {

enum ModelType : uint32_t { ModelType_Background = 1, ModelType_Character = 2 };

enum MeshFlag : uint32_t { MeshFlag_DoubleSided = (1 << 0) };

int const ModelMaxChildrenPerBone = 134;
int const ModelMaxMorphTargetsPerModel = 256;
int const ModelMaxMorphTargetsPerMesh = 32;
int const ModelUnknownsAfterMorphTargets = 12;
// TODO: How do we actually want to do this?
// Character models generally have <300 bones. Some background models have >600
// bones (what are these for? - some of them seem broken).
// Dynamic array?
int const ModelMaxBonesPerModel = 768;
int const ModelMaxBonesPerMesh = 32;
int const ModelMaxMeshesPerModel = 32;
int const ModelMaxRootBones = 32;
int const ModelMaxTexturesPerModel = 32;

extern uint32_t* g_ModelIds;
extern char** g_ModelNames;
extern uint32_t g_ModelCount;

extern uint32_t* g_BackgroundModelIds;
extern char** g_BackgroundModelNames;
extern uint32_t g_BackgroundModelCount;

extern uint32_t* g_AnimationIds;
extern char** g_AnimationNames;
extern uint32_t g_AnimationCount;

struct VertexBuffer {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 UV;
  uint8_t BoneIndices[4];  // indices into Mesh.BoneMap
  glm::vec4 BoneWeights;
};

struct BgVertexBuffer {
  glm::vec3 Position;
  glm::vec2 UV;
};

struct MorphVertexBuffer {
  glm::vec3 Position;
  glm::vec3 Normal;
};

struct MorphTarget {
  int32_t VertexCount;
  // Offset (in sizeof(MorphVertexBuffer) units) into Model.MorphVertexBuffers
  int32_t VertexOffset;
};

struct StaticBone {
  int16_t Id;
  int16_t Parent;
  int16_t ChildrenCount;
  int16_t Children[ModelMaxChildrenPerBone];  // TODO check

  Transform BaseTransform;

  glm::mat4 BindInverse;
};

enum TextureType {
  TT_ColorMap = 0,
  TT_GradientMaskMap = 2,
  TT_SpecularColorMap = 4,
  TT_Character3D_Max = 4,

  TT_Eye_IrisColorMap = 0,
  TT_Eye_WhiteColorMap = 1,
  TT_Eye_IrisSpecularColorMap = 2,
  TT_Eye_HighlightColorMap = 3,
  TT_Eye_Max = 3,

  TT_Count = 6
};

struct Mesh {
  // Meshes in one group are animated together
  int32_t GroupId;

  int32_t Id;

  int32_t VertexCount;
  // Offset (in sizeof(VertexBuffer) units) into Model.VertexBuffers
  int32_t VertexOffset;
  int32_t IndexCount;
  // Offset (in sizeof(uint16_t) units) into Model.Indices
  int32_t IndexOffset;

  uint32_t Flags;
  float Opacity;

  // This is only used with background models
  Transform ModelTransform;

  uint8_t MorphTargetIds[ModelMaxMorphTargetsPerMesh];

  uint32_t UsedBones;                     // 0 => not skinned
  int16_t BoneMap[ModelMaxBonesPerMesh];  // indices into Model.Bones

  int16_t Maps[TT_Count];

  // Sets transform for meshes without individual vertex skinning
  int16_t MeshBone;

  uint8_t MorphTargetCount;
};

class Animation;

class Model {
 public:
  static void Init();

  static void UnloadAnimations();

  // Parses a R;NE model file. No GPU submission happens in this class.
  static Model* Load(uint32_t modelId);
  // Ground plane
  static Model* MakePlane();

  ~Model();

  uint32_t Id;

  ModelType Type;

  int32_t MeshCount = 0;
  Mesh Meshes[ModelMaxMeshesPerModel];

  int32_t VertexCount = 0;
  void* VertexBuffers = 0;

  int32_t MorphTargetCount = 0;
  MorphTarget MorphTargets[ModelMaxMorphTargetsPerModel];

  int32_t MorphVertexCount = 0;
  MorphVertexBuffer* MorphVertexBuffers = 0;

  int32_t IndexCount = 0;
  uint16_t* Indices = 0;

  int32_t TextureCount = 0;
  Texture Textures[ModelMaxTexturesPerModel];

  int32_t BoneCount = 0;
  StaticBone Bones[ModelMaxBonesPerModel];

  int32_t RootBoneCount = 0;
  int16_t RootBones[ModelMaxRootBones];

  ska::flat_hash_map<uint16_t, Animation*> Animations;
};

}  // namespace Impacto