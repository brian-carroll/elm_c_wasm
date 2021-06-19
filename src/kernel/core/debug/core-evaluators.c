char* Debug_evaluator_name_core(void* p) {
  if (p == Basics_add.evaluator) {
    return "Basics_add";
  }
  if (p == Basics_sub.evaluator) {
    return "Basics_sub";
  }
  if (p == Basics_mul.evaluator) {
    return "Basics_mul";
  }
  if (p == Basics_fdiv.evaluator) {
    return "Basics_fdiv";
  }
  if (p == Basics_idiv.evaluator) {
    return "Basics_idiv";
  }
  if (p == Basics_pow.evaluator) {
    return "Basics_pow";
  }
  if (p == Basics_toFloat.evaluator) {
    return "Basics_toFloat";
  }
  if (p == Basics_round.evaluator) {
    return "Basics_round";
  }
  if (p == Basics_floor.evaluator) {
    return "Basics_floor";
  }
  if (p == Basics_ceiling.evaluator) {
    return "Basics_ceiling";
  }
  if (p == Basics_not.evaluator) {
    return "Basics_not";
  }
  if (p == Basics_and.evaluator) {
    return "Basics_and";
  }
  if (p == Basics_or.evaluator) {
    return "Basics_or";
  }
  if (p == Basics_xor.evaluator) {
    return "Basics_xor";
  }
  if (p == Basics_modBy.evaluator) {
    return "Basics_modBy";
  }
  if (p == Basics_remainderBy.evaluator) {
    return "Basics_remainderBy";
  }
  if (p == Basics_log.evaluator) {
    return "Basics_log";
  }
  if (p == Basics_identity.evaluator) {
    return "Basics_identity";
  }
  if (p == Bitwise_and.evaluator) {
    return "Bitwise_and";
  }
  if (p == Bitwise_or.evaluator) {
    return "Bitwise_or";
  }
  if (p == Bitwise_xor.evaluator) {
    return "Bitwise_xor";
  }
  if (p == Bitwise_complement.evaluator) {
    return "Bitwise_complement";
  }
  if (p == Bitwise_shiftLeftBy.evaluator) {
    return "Bitwise_shiftLeftBy";
  }
  if (p == Bitwise_shiftRightBy.evaluator) {
    return "Bitwise_shiftRightBy";
  }
  if (p == Bitwise_shiftRightZfBy.evaluator) {
    return "Bitwise_shiftRightZfBy";
  }
  if (p == Char_toCode.evaluator) {
    return "Char_toCode";
  }
  if (p == g_elm_core_Maybe_Just.evaluator) {
    return "g_elm_core_Maybe_Just";
  }
  if (p == g_elm_core_Array_initialize.evaluator) {
    return "g_elm_core_Array_initialize";
  }
  if (p == g_elm_core_List_reverse.evaluator) {
    return "g_elm_core_List_reverse";
  }
  if (p == g_elm_core_List_foldl.evaluator) {
    return "g_elm_core_List_foldl";
  }
  if (p == JsArray_singleton.evaluator) {
    return "JsArray_singleton";
  }
  if (p == JsArray_length.evaluator) {
    return "JsArray_length";
  }
  if (p == JsArray_initialize.evaluator) {
    return "JsArray_initialize";
  }
  if (p == JsArray_initializeFromList.evaluator) {
    return "JsArray_initializeFromList";
  }
  if (p == JsArray_unsafeGet.evaluator) {
    return "JsArray_unsafeGet";
  }
  if (p == JsArray_unsafeSet.evaluator) {
    return "JsArray_unsafeSet";
  }
  if (p == JsArray_push.evaluator) {
    return "JsArray_push";
  }
  if (p == JsArray_foldl.evaluator) {
    return "JsArray_foldl";
  }
  if (p == JsArray_foldr.evaluator) {
    return "JsArray_foldr";
  }
  if (p == JsArray_map.evaluator) {
    return "JsArray_map";
  }
  if (p == JsArray_indexedMap.evaluator) {
    return "JsArray_indexedMap";
  }
  if (p == JsArray_slice.evaluator) {
    return "JsArray_slice";
  }
  if (p == JsArray_appendN.evaluator) {
    return "JsArray_appendN";
  }
  if (p == List_cons.evaluator) {
    return "List_cons";
  }
  if (p == List_append.evaluator) {
    return "List_append";
  }
  if (p == List_map2.evaluator) {
    return "List_map2";
  }
  if (p == List_sortBy.evaluator) {
    return "List_sortBy";
  }
  if (p == Platform_batch.evaluator) {
    return "Platform_batch";
  }
  if (p == Platform_leaf.evaluator) {
    return "Platform_leaf";
  }
  if (p == Platform_map.evaluator) {
    return "Platform_map";
  }
  if (p == Platform_sendToApp.evaluator) {
    return "Platform_sendToApp";
  }
  if (p == Platform_sendToSelf.evaluator) {
    return "Platform_sendToSelf";
  }
  if (p == eval_sendToApp_revArgs) {
    return "eval_sendToApp_revArgs";
  }
  if (p == Scheduler_succeed.evaluator) {
    return "Scheduler_succeed";
  }
  if (p == Scheduler_fail.evaluator) {
    return "Scheduler_fail";
  }
  if (p == Scheduler_binding.evaluator) {
    return "Scheduler_binding";
  }
  if (p == Scheduler_andThen.evaluator) {
    return "Scheduler_andThen";
  }
  if (p == Scheduler_onError.evaluator) {
    return "Scheduler_onError";
  }
  if (p == Scheduler_receive.evaluator) {
    return "Scheduler_receive";
  }
  if (p == Scheduler_rawSpawn.evaluator) {
    return "Scheduler_rawSpawn";
  }
  if (p == Scheduler_spawn.evaluator) {
    return "Scheduler_spawn";
  }
  if (p == Scheduler_rawSend.evaluator) {
    return "Scheduler_rawSend";
  }
  if (p == Scheduler_send.evaluator) {
    return "Scheduler_send";
  }
  if (p == Scheduler_kill.evaluator) {
    return "Scheduler_kill";
  }
  if (p == String_uncons.evaluator) {
    return "String_uncons";
  }
  if (p == String_append.evaluator) {
    return "String_append";
  }
  if (p == String_length.evaluator) {
    return "String_length";
  }
  if (p == String_foldr.evaluator) {
    return "String_foldr";
  }
  if (p == String_split.evaluator) {
    return "String_split";
  }
  if (p == String_join.evaluator) {
    return "String_join";
  }
  if (p == String_slice.evaluator) {
    return "String_slice";
  }
  if (p == String_trim.evaluator) {
    return "String_trim";
  }
  if (p == String_trimLeft.evaluator) {
    return "String_trimLeft";
  }
  if (p == String_trimRight.evaluator) {
    return "String_trimRight";
  }
  if (p == String_all.evaluator) {
    return "String_all";
  }
  if (p == String_contains.evaluator) {
    return "String_contains";
  }
  if (p == String_startsWith.evaluator) {
    return "String_startsWith";
  }
  if (p == String_endsWith.evaluator) {
    return "String_endsWith";
  }
  if (p == String_indexes.evaluator) {
    return "String_indexes";
  }
  if (p == String_fromNumber.evaluator) {
    return "String_fromNumber";
  }
  if (p == String_toInt.evaluator) {
    return "String_toInt";
  }
  if (p == String_toFloat.evaluator) {
    return "String_toFloat";
  }
  if (p == Utils_equal.evaluator) {
    return "Utils_equal";
  }
  if (p == Utils_notEqual.evaluator) {
    return "Utils_notEqual";
  }
  if (p == Utils_append.evaluator) {
    return "Utils_append";
  }
  if (p == Utils_compare.evaluator) {
    return "Utils_compare";
  }
  if (p == Utils_lt.evaluator) {
    return "Utils_lt";
  }
  if (p == Utils_le.evaluator) {
    return "Utils_le";
  }
  if (p == Utils_gt.evaluator) {
    return "Utils_gt";
  }
  if (p == Utils_ge.evaluator) {
    return "Utils_ge";
  }
  if (p == eval_elm_core_Result_Ok) {
    return "eval_elm_core_Result_Ok";
  }
  if (p == eval_elm_core_Result_Err) {
    return "eval_elm_core_Result_Err";
  }
  if (p == eval_elm_core_Result_isOk) {
    return "eval_elm_core_Result_isOk";
  }
  if (p == eval_List_append) {
    return "eval_List_append";
  }
  if (p == eval_Platform_leaf) {
    return "eval_Platform_leaf";
  }
  if (p == eval_Platform_sendToApp) {
    return "eval_Platform_sendToApp";
  }
  if (p == eval_Platform_sendToSelf) {
    return "eval_Platform_sendToSelf";
  }
  if (p == eval_Scheduler_rawSpawn) {
    return "eval_Scheduler_rawSpawn";
  }
  if (p == eval_Scheduler_spawn) {
    return "eval_Scheduler_spawn";
  }
  if (p == eval_Scheduler_succeed) {
    return "eval_Scheduler_succeed";
  }
  if (p == eval_Scheduler_fail) {
    return "eval_Scheduler_fail";
  }
  if (p == eval_Scheduler_binding) {
    return "eval_Scheduler_binding";
  }
  if (p == eval_Scheduler_andThen) {
    return "eval_Scheduler_andThen";
  }
  if (p == eval_Scheduler_onError) {
    return "eval_Scheduler_onError";
  }
  if (p == eval_Scheduler_receive) {
    return "eval_Scheduler_receive";
  }
  if (p == eval_Scheduler_rawSend) {
    return "eval_Scheduler_rawSend";
  }
  if (p == eval_Scheduler_send) {
    return "eval_Scheduler_send";
  }
  if (p == eval_Scheduler_kill) {
    return "eval_Scheduler_kill";
  }
  if (p == eval_String_append) {
    return "eval_String_append";
  }
  if (p == Json_succeed.evaluator) {
    return "Json_succeed";
  }
  if (p == Json_fail.evaluator) {
    return "Json_fail";
  }
  if (p == Json_decodeList.evaluator) {
    return "Json_decodeList";
  }
  if (p == Json_decodeArray.evaluator) {
    return "Json_decodeArray";
  }
  if (p == Json_decodeNull.evaluator) {
    return "Json_decodeNull";
  }
  if (p == Json_decodeField.evaluator) {
    return "Json_decodeField";
  }
  if (p == Json_decodeIndex.evaluator) {
    return "Json_decodeIndex";
  }
  if (p == Json_decodeKeyValuePairs.evaluator) {
    return "Json_decodeKeyValuePairs";
  }
  if (p == Json_andThen.evaluator) {
    return "Json_andThen";
  }
  if (p == Json_oneOf.evaluator) {
    return "Json_oneOf";
  }
  if (p == Json_map1.evaluator) {
    return "Json_map1";
  }
  if (p == Json_map2.evaluator) {
    return "Json_map2";
  }
  if (p == Json_map3.evaluator) {
    return "Json_map3";
  }
  if (p == Json_map4.evaluator) {
    return "Json_map4";
  }
  if (p == Json_map5.evaluator) {
    return "Json_map5";
  }
  if (p == Json_map6.evaluator) {
    return "Json_map6";
  }
  if (p == Json_map7.evaluator) {
    return "Json_map7";
  }
  if (p == Json_map8.evaluator) {
    return "Json_map8";
  }
  if (p == Json_run.evaluator) {
    return "Json_run";
  }
  if (p == Json_runOnString.evaluator) {
    return "Json_runOnString";
  }
  if (p == Json_wrap.evaluator) {
    return "Json_wrap";
  }
  if (p == Json_unwrap.evaluator) {
    return "Json_unwrap";
  }
  if (p == Json_emptyArray.evaluator) {
    return "Json_emptyArray";
  }
  if (p == Json_emptyObject.evaluator) {
    return "Json_emptyObject";
  }
  if (p == Json_addField.evaluator) {
    return "Json_addField";
  }
  if (p == Json_addEntry.evaluator) {
    return "Json_addEntry";
  }
  if (p == Json_encode.evaluator) {
    return "Json_encode";
  }
  return "(unknown evaluator)";
}
