layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in lowp ivec4 BoneIndices;  // indices into Mesh.BoneMap
layout(location = 4) in vec4 BoneWeights;

out vec3 worldFragPosition;
out vec3 worldNormal;
out vec2 uv;

layout(std140) uniform Character3DCommon {
  UNIFORM_PRECISION mat4 ViewProjection;
  UNIFORM_PRECISION mat4 Model;
  UNIFORM_PRECISION mat4 Bones[ModelMaxBonesPerMesh];
  UNIFORM_PRECISION vec4 Tint;
  UNIFORM_PRECISION vec3 WorldLightPosition;
  UNIFORM_PRECISION vec3 WorldEyePosition;
  UNIFORM_PRECISION float ModelOpacity;
};

void main() {
  // Accumulated skinning, thanks
  // https://developer.nvidia.com/gpugems/GPUGems/gpugems_ch04.html
  mat4 skeletalTransform = Bones[BoneIndices.x] * BoneWeights.x +
                           Bones[BoneIndices.y] * BoneWeights.y +
                           Bones[BoneIndices.z] * BoneWeights.z +
                           Bones[BoneIndices.w] * BoneWeights.w;

  mat4 transform = Model * skeletalTransform;
  mat3 normalMatrix = mat3(transpose(inverse(transform)));

  vec4 worldPosition = transform * vec4(Position, 1.0);
  worldFragPosition = worldPosition.xyz;

  gl_Position = ViewProjection * worldPosition;
  worldNormal = normalMatrix * Normal;
  uv = UV;
}