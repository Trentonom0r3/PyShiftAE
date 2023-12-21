#include "EffectSuites.h"

Result<int> GetLayerNumEffects(Result<AEGP_LayerH> layerH) {
  A_long num_effectsPL;
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_LayerH layer = layerH.value;
  ERR(suites.EffectSuite4()->AEGP_GetLayerNumEffects(layer, &num_effectsPL));
  return num_effectsPL;
}

Result<AEGP_EffectRefH> GetLayerEffectByIndex(Result<AEGP_LayerH> layerH, int effect_indexL) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_LayerH layer = layerH.value;
  AEGP_EffectIndex effect_index = effect_indexL;
  AEGP_EffectRefH effectPH;
  AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();

  if (pluginIDPtr != nullptr) {
	  // Dereference the pointer to get the plugin ID
	  AEGP_PluginID  pluginID = *pluginIDPtr;
	  ERR(suites.EffectSuite4()->AEGP_GetLayerEffectByIndex(pluginID, layer, effect_index, &effectPH));
  }
  if (err != A_Err_NONE) {
	  return Result<AEGP_EffectRefH>(nullptr, err);
  }
	Result<AEGP_EffectRefH> result(effectPH, err);
	return result;
}

Result<AEGP_InstalledEffectKey> GetInstalledKeyFromLayerEffect(Result<AEGP_EffectRefH> effect_refH) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_EffectRefH effect_ref = effect_refH.value;
  AEGP_InstalledEffectKey installed_keyP;
  ERR(suites.EffectSuite4()->AEGP_GetInstalledKeyFromLayerEffect(effect_ref, &installed_keyP));
  return installed_keyP;
}

Result<PF_ParamDefUnion> GetEffectParamUnionByIndex(Result<AEGP_EffectRefH> effectH, int param_index) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_EffectRefH effect = effectH.value;
  PF_ParamIndex param_indexP = param_index;
  PF_ParamType param_typeP;
  PF_ParamDefUnion uP0;
  ERR(suites.EffectSuite4()->AEGP_GetEffectParamUnionByIndex(*SuiteManager::GetInstance().GetPluginID(), effect, param_indexP, &param_typeP, &uP0));
  Result<PF_ParamDefUnion> result(uP0, err);
return result;
}

Result<AEGP_EffectFlags> GetEffectFlags(Result<AEGP_EffectRefH> effect_refH) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_EffectRefH effect_ref = effect_refH.value;
  AEGP_EffectFlags effect_flagsP;
  ERR(suites.EffectSuite4()->AEGP_GetEffectFlags(effect_ref, &effect_flagsP));
  return effect_flagsP;
}

Result<void> SetEffectFlags(Result<AEGP_EffectRefH> effect_refH, AEGP_EffectFlags mask, AEGP_EffectFlags effect_flags) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_EffectRefH effect_ref = effect_refH.value;
  ERR(suites.EffectSuite4()->AEGP_SetEffectFlags(effect_ref, mask, effect_flags));
return Result<void>(err);
}

Result<void> ReorderEffect(Result<AEGP_EffectRefH> effect_refH, int effect_indexL) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  A_long effect_index = effect_indexL;
  AEGP_EffectRefH effect_ref = effect_refH.value;
  ERR(suites.EffectSuite4()->AEGP_ReorderEffect(effect_ref, effect_index));
  return Result<void>(err);
}

Result<void> EffectCallGeneric(Result<AEGP_EffectRefH> effectH, void* extraPV) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  A_Time timeP;
  timeP.scale = 1;
  timeP.value = 0;
  AEGP_EffectRefH effect = effectH.value;
  AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();

  if (pluginIDPtr != nullptr) {
	  // Dereference the pointer to get the plugin ID
	  AEGP_PluginID  pluginID = *pluginIDPtr;
	  ERR(suites.EffectSuite4()->AEGP_EffectCallGeneric(pluginID, effect, &timeP, PF_Cmd_COMPLETELY_GENERAL, extraPV));
	  return Result<void>(err);
  }
}

Result<void> DisposeEffect(Result<AEGP_EffectRefH> effectH) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_EffectRefH effect = effectH.value;
  ERR(suites.EffectSuite4()->AEGP_DisposeEffect(effect));
  return Result<void>(err);
}

Result<AEGP_EffectRefH> ApplyEffect(Result<AEGP_LayerH> layerH, Result<AEGP_InstalledEffectKey> installed_keyZ) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_LayerH layer = layerH.value;
  AEGP_EffectRefH effect_refPH;
  AEGP_InstalledEffectKey installed_key = installed_keyZ.value;
  ERR(suites.EffectSuite4()->AEGP_ApplyEffect(*SuiteManager::GetInstance().GetPluginID(), layer, installed_key, &effect_refPH));
  Result<AEGP_EffectRefH> result(effect_refPH, err);
  return result;
}

Result<void> DeleteLayerEffect(Result<AEGP_EffectRefH> effect_refH) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_DeleteLayerEffect(effect_refH.value));
  return Result<void>(err);
}

Result<int> GetNumInstalledEffects() {
  A_Err err = A_Err_NONE;
  A_long num_installed_effectsPL;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_GetNumInstalledEffects(&num_installed_effectsPL));
  return Result<int>(num_installed_effectsPL, err);
}

Result<AEGP_InstalledEffectKey> GetNextInstalledEffect(Result<AEGP_InstalledEffectKey> key) {
  A_Err err = A_Err_NONE;
  AEGP_InstalledEffectKey next_keyPH;
  AEGP_InstalledEffectKey keyP = key.value;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_GetNextInstalledEffect(keyP, &next_keyPH));
  return Result<AEGP_InstalledEffectKey>(next_keyPH, err);
}

Result<void> GetEffectName(AEGP_InstalledEffectKey installed_key, A_char* nameZ) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_GetEffectName(installed_key, nameZ));
  return Result<void>(err);
}

Result<std::string> GetEffectMatchName(Result<AEGP_InstalledEffectKey> installed_keyZ){
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  A_char match_nameZ[AEGP_MAX_EFFECT_MATCH_NAME_SIZE] = { '\0' };
  AEGP_InstalledEffectKey key = installed_keyZ.value;
  ERR(suites.EffectSuite4()->AEGP_GetEffectMatchName(key, match_nameZ));
  std::string name = match_nameZ;
  return Result<std::string>(name, err);
}

Result<void> GetEffectCategory(Result<AEGP_InstalledEffectKey>  installed_key, A_char* categoryZ) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  AEGP_InstalledEffectKey installed_keyZ = installed_key.value;
  ERR(suites.EffectSuite4()->AEGP_GetEffectCategory(installed_keyZ, categoryZ));
  return Result<void>(err);
}

Result<AEGP_EffectRefH> DuplicateEffect(Result<AEGP_EffectRefH> orig_effect_refH) {
  A_Err err = A_Err_NONE;
  AEGP_EffectRefH dupe_refPH;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_DuplicateEffect(orig_effect_refH.value, &dupe_refPH));
  Result<AEGP_EffectRefH> result(dupe_refPH, err);
  return result;
}

Result<int> NumEffectMask(Result<AEGP_EffectRefH> effect_refH) {
  A_Err err = A_Err_NONE;
  A_u_long num_masksPL;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_NumEffectMask(effect_refH.value, &num_masksPL));
  return num_masksPL;
}

Result<AEGP_MaskIDVal> GetEffectMaskID(Result<AEGP_EffectRefH> effect_refH, int mask_indexL) {
  A_Err err = A_Err_NONE;
  AEGP_MaskIDVal id_valP;
  A_u_long mask_index = mask_indexL;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_GetEffectMaskID(effect_refH.value, mask_index, &id_valP));
  return id_valP;
}

Result<AEGP_StreamRefH> AddEffectMask(Result<AEGP_EffectRefH> effect_refH, AEGP_MaskIDVal id_val) {
  A_Err err = A_Err_NONE;
  AEGP_StreamRefH streamPH0;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_AddEffectMask(effect_refH.value, id_val, &streamPH0));
  Result<AEGP_StreamRefH> result(streamPH0, err);
  return result;
}

Result<void> RemoveEffectMask(Result<AEGP_EffectRefH> effect_refH, AEGP_MaskIDVal id_val) {
  A_Err err = A_Err_NONE;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_RemoveEffectMask(effect_refH.value, id_val));
  return Result<void>(err);
}

Result<AEGP_StreamRefH> SetEffectMask(Result<AEGP_EffectRefH> effect_refH, int mask_indexL, AEGP_MaskIDVal id_val) {
  A_Err err = A_Err_NONE;
  AEGP_StreamRefH streamPH0;
  A_u_long mask_index = mask_indexL;
  AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
  ERR(suites.EffectSuite4()->AEGP_SetEffectMask(effect_refH.value, mask_index, id_val, &streamPH0));
  Result<AEGP_StreamRefH> result(streamPH0, err);
  return result;
}






