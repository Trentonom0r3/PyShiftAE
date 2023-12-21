#pragma once
#include "Core.h"

Result<int> GetLayerNumEffects(Result<AEGP_LayerH> layerH);

Result<AEGP_EffectRefH> GetLayerEffectByIndex(Result<AEGP_LayerH> layerH, int effect_indexL);

Result<AEGP_InstalledEffectKey> GetInstalledKeyFromLayerEffect(Result<AEGP_EffectRefH> effect_refH);

Result<PF_ParamDefUnion> GetEffectParamUnionByIndex(Result<AEGP_EffectRefH> effectH, int param_index);

Result<AEGP_EffectFlags> GetEffectFlags(Result<AEGP_EffectRefH> effect_refH);

Result<void> SetEffectFlags(Result<AEGP_EffectRefH> effect_refH, AEGP_EffectFlags mask, AEGP_EffectFlags effect_flags);

Result<void> ReorderEffect(Result<AEGP_EffectRefH> effect_refH, int effect_indexL);

Result<void> EffectCallGeneric(Result<AEGP_EffectRefH> effectH, void* extraPV);

Result<void> DisposeEffect(Result<AEGP_EffectRefH> effectH);

Result<AEGP_EffectRefH> ApplyEffect(Result<AEGP_LayerH> layerH, Result<AEGP_InstalledEffectKey> installed_key);

Result<void> DeleteLayerEffect(Result<AEGP_EffectRefH> effect_refH);

Result<int> GetNumInstalledEffects();

Result<AEGP_InstalledEffectKey> GetNextInstalledEffect(Result<AEGP_InstalledEffectKey> key);

Result<void> GetEffectName(AEGP_InstalledEffectKey installed_key, A_char* nameZ);

Result<std::string> GetEffectMatchName(Result<AEGP_InstalledEffectKey> installed_keyZ);

Result<void> GetEffectCategory(AEGP_InstalledEffectKey installed_key, A_char* categoryZ);

Result<AEGP_EffectRefH> DuplicateEffect(Result<AEGP_EffectRefH> orig_effect_refH);

Result<int> NumEffectMask(Result<AEGP_EffectRefH> effect_refH);

Result<AEGP_MaskIDVal> GetEffectMaskID(Result<AEGP_EffectRefH> effect_refH, int mask_indexL);

Result<AEGP_StreamRefH> AddEffectMask(Result<AEGP_EffectRefH> effect_refH, AEGP_MaskIDVal id_val);

Result<void> RemoveEffectMask(Result<AEGP_EffectRefH> effect_refH, AEGP_MaskIDVal id_val);

Result<AEGP_StreamRefH> SetEffectMask(Result<AEGP_EffectRefH> effect_refH, int mask_indexL, AEGP_MaskIDVal id_val);






