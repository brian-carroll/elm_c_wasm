function _Platform_initialize(flagDecoder, args, init, update, subscriptions, stepperBuilder)
{
  // flagDecoder is an compiler-generated JS function. Keep the decoding in JS
	var flagsResult = A2(_Json_run, flagDecoder, _Json_wrap(args ? args['flags'] : undefined));
	_Result_isOk(flagsResult) || _Debug_crash(2, _Json_errorToString(flagsResult.a));
  var managers = {};

	var model_cmd = init(decodedFlags); // JS wrapper with closed-over URL and Browser.key
	var model = model_cmd.a;
	var stepper = stepperBuilder(sendToApp, model); // 


  // note changes in core v1.0.5


  // _Platform_setupEffects iterates over a load-time dict of manager configs (constructed by _Platform_createManager) and instantiates them
  // instantiation means (1) create a router (2) set up the "process" for the manager, which is an async infinite loop 
	var ports = _Platform_setupEffects(managers, sendToApp);


  // viewMetadata is never used in Browser or virtual-dom, debugger, or anywhere else I can find
	function sendToApp(msg, viewMetadata)
	{
		model_cmd = A2(update, msg, model);
		stepper(model = model_cmd.a, viewMetadata); // stepper is a JS wrapper around view (from _Browser_makeAnimator). It mutates model, which seems a bit rude
		_Platform_dispatchEffects(managers, model_cmd.b, subscriptions(model));
	}

  // note changes in core v1.0.5: dispatchEffects replaced by enqueueEffects

	_Platform_dispatchEffects(managers, model_cmd.b, subscriptions(model));

	return ports ? { ports: ports } : {};
}

/*
Effect manager home could be a C enum. Already have lots of compiler magic for Platform.

_Platform_dispatchEffects (Wasm)
   - calls _Platform_gatherEffects
   - does rawSend per manager


_Platform_gatherEffects (Wasm)
  - Mutates effectsDict
  - traverses the tree of effects and gathers into a dict per manager
    - flattens batches
    - does Platform_map stuff (building a linked list of tagger functions)
  - calls _Platform_toEffect which applies the taggers
    - each manager defines its own Functor with a `map` function _in Elm code_
    - subMap : (a -> b) -> MySub a -> MySub b


_Platform_toEffect (Wasm)
  - Pure helper function, only called from gatherEffects
  - If required by Cmd.map or Sub.map, build up a Closure by repeated application of the manager-specific (Elm) map function
  - The map function is applied to the value of the Cmd or Sub _at construction_
      - Typically, the effect's 'value' will contain a function, to be called on some future result, and `map` will involve function composition


_Platform_insert (Wasm)
  - Pure helper function, only called from gatherEffects


_Platform_createManager (Wasm)
  - constructor for a kernel union.
  - Gets called at load time with some Elm functions and put into the dict of manager configs.
    ```js
        _Platform_effectManagers['Task'] = _Platform_createManager(
          $elm$core$Task$init,
          $elm$core$Task$onEffects,
          $elm$core$Task$onSelfMsg,
          $elm$core$Task$cmdMap
        );
    ```


_Platform_setupEffects (JS & Wasm)
  - called from initialize
  - for each manager config (constructed by _Platform_createManager)
    - if the manager has __portSetup function, then call it
    - instantiate the manager using _Platform_instantiateManager
  - returns ports


_Platform_instantiateManager (Wasm)
  (1) create a router (between a manager and an app)
  (2) set up the "process" for the manager, which is an async infinite loop for a state-machine, advanced by messages via router


_Platform_sendToApp (Wasm)
  Elm interface to sendToApp, wrapped in a Task using __Scheduler_binding and a router


_Platform_sendToSelf (Wasm)
  Elm function allowing manager code to send itself messages to update its internal state


_Platform_leaf, _Platform_batch, _Platform_map (Wasm)
  Cmd/Sub constructors


_Platform_outgoingPort (Wasm)
  - Manager config constructor


_Platform_outgoingPortMap (Wasm)
  - dummy mapper


_Platform_setupOutgoingPort (JS & Wasm)
  - called from _Platform_setupEffects via __portSetup
  - Reads global variable _Platform_effectManagers
  - Gets auto-generated JSON encoder if any! (leave this in JS)
  - Sets up the onEffects function, which for _any_ other manager would be Elm code
    - Handles Cmds from the app. Converts an Elm value to Json.Value
    - Calls all subscribers in JS-land
  - This should stay mostly in JS except for the manager config stuff that would normally be Wasm


_Platform_incomingPort (Wasm)
  - Manager config constructor

_Platform_incomingPortMap (Wasm)
  - Manager config property, used in _Platform_incomingPort

_Platform_setupIncomingPort (JS & Wasm)
  - has an auto-generated Json decoder
  - everything else can be Wasm

_Platform_export (JS)
_Platform_mergeExports (JS)
*/