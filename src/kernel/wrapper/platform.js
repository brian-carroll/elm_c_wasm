function _Platform_initialize(flagDecoder, args, init, update, subscriptions, stepperBuilder)
{
  // flagDecoder is an compiler-generated JS function. Keep the decoding in JS
	var flagsResult = A2(_Json_run, flagDecoder, _Json_wrap(args ? args['flags'] : undefined));
	_Result_isOk(flagsResult) || _Debug_crash(2, _Json_errorToString(flagsResult.a));
  var managers = {};

	var model_cmd = init(decodedFlags); // JS wrapper with closed-over URL and Browser.key
	var model = model_cmd.a;
	var stepper = stepperBuilder(sendToApp, model);
	var ports = _Platform_setupEffects(managers, sendToApp); // iterates over a load-time config object and instantiates each manager onto 'managers'

	function sendToApp(msg, viewMetadata)
	{
		model_cmd = A2(update, msg, model);
		stepper(model = model_cmd.a, viewMetadata);
		_Platform_dispatchEffects(managers, model_cmd.b, subscriptions(model));
	}

	_Platform_dispatchEffects(managers, model_cmd.b, subscriptions(model));

	return ports ? { ports: ports } : {};
}

/*
_Platform_dispatchEffects
   - calls _Platform_gatherEffects
   - does rawSend per manager

_Platform_gatherEffects
  - traverses the tree of effects and gathers into a dict per manager
    - flattens batches
    - does Platform_map stuff (building a linked list of tagger functions)
  - calls _Platform_toEffect which applies the taggers
    - each manager defines its own Functor with a `map` function _in Elm code_
    - subMap : (a -> b) -> MySub a -> MySub b

_Platform_toEffect


_Platform_insert


_Platform_createManager is a constructor for a kernel union. It gets called at load time with some Elm functions and put into the dict of manager configs.

_Platform_effectManagers['Task'] = _Platform_createManager(
  $elm$core$Task$init,
  $elm$core$Task$onEffects,
  $elm$core$Task$onSelfMsg,
  $elm$core$Task$cmdMap
);


*/