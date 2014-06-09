#include "ln_common.h"

AI_SHADER_NODE_EXPORT_METHODS(BillowMtd);

enum BillowParams
{
   p_input = 0,
   p_custom_input,
   p_amplitude,
   p_frequency,
   p_octaves,
   p_persistence,
   p_lacunarity,
   p_seed,
   p_quality
};

node_parameters
{
   AiParameterEnum("input", I_P, InputNames);
   AiParameterPnt("custom_input", 0.0f, 0.0f, 0.0f);
   AiParameterFlt("amplitude", 1.0f);
   AiParameterFlt("frequency", 1.0f);
   AiParameterInt("octaves", 6);
   AiParameterFlt("persistence", 0.5f);
   AiParameterFlt("lacunarity", 2.0f);
   AiParameterInt("seed", 0);
   AiParameterEnum("quality", NQ_std, NoiseQualityNames);
   
   AiMetaDataSetBool(mds, "quality", "linkable", false);
   AiMetaDataSetBool(mds, "input", "linkable", false);
}

node_initialize
{
}

node_update
{
   AiNodeSetLocalData(node, (void*) (AiNodeIsLinked(node, "custom_input") ? 1 : 0));
}

node_finish
{
}

shader_evaluate
{
   bool is_input_linked = (AiNodeGetLocalData(node) == (void*)1);
   
   float amplitude = AiShaderEvalParamFlt(p_amplitude);
   float frequency = AiShaderEvalParamFlt(p_frequency);
   float lacunarity = AiShaderEvalParamFlt(p_lacunarity);
   int octaves = AiShaderEvalParamInt(p_octaves);
   float persistence = AiShaderEvalParamFlt(p_persistence);
   NoiseQuality quality = (NoiseQuality) AiShaderEvalParamInt(p_quality);
   int seed = AiShaderEvalParamInt(p_seed);
   Input input = (Input) AiShaderEvalParamInt(p_input);
   
   AtPoint P = (is_input_linked ? AiShaderEvalParamPnt(p_custom_input) : GetInput(input, sg, node));
   
   sg->out.FLT = AbsFractal(P, octaves, amplitude, persistence, frequency, lacunarity, seed, quality);
   
   sg->out.FLT = std::max(0.0f, 0.5f * (1.0f + sg->out.FLT));
}
