
// INITIALIZE A PROGRAM

scope.perfTimes = [];
function _Platform_initialize(flagDecoder, args, init, update, subscriptions, stepperBuilder)
{
	var result = A2(_Json_run, flagDecoder, _Json_wrap(args ? args['flags'] : undefined));
	$elm$core$Result$isOk(result) || _Debug_crash(2 /**/, _Json_errorToString(result.a) /**/);
	var managers = {};
	result = init(result.a);
	var model = result.a;
	var stepper = stepperBuilder(sendToApp, model);
	var ports = _Platform_setupEffects(managers, sendToApp);

	function sendToApp(msg, viewMetadata)
	{
    const tStart = performance.now();
		result = A2(update, msg, model);
    const tUpdate = performance.now();
		stepper(model = result.a, viewMetadata);
    const tStepper = performance.now();
		_Platform_dispatchEffects(managers, result.b, subscriptions(model));
    const tEffects = performance.now();
    const o = {
      msgCtor: [msg.$, msg.a.$],
      tUpdate: tUpdate - tStart,
      tStepper: tStepper - tUpdate,
      tEffects: tEffects - tStepper,
    };
    scope.perfTimes.push(o);
    console.log(o)
	}

	_Platform_dispatchEffects(managers, result.b, subscriptions(model));

	return ports ? { ports: ports } : {};
}

// DOCUMENT


var _Debugger_document;

var _Browser_document = _Debugger_document || F4(function(impl, flagDecoder, debugMetadata, args)
{
	return _Platform_initialize(
		flagDecoder,
		args,
		impl.init,
		impl.update,
		impl.subscriptions,
		function(sendToApp, initialModel) {
			var divertHrefToApp = impl.setup && impl.setup(sendToApp)
			var view = impl.view;
			var title = _VirtualDom_doc.title;
			var bodyNode = _VirtualDom_doc.body;
			var currNode = _VirtualDom_virtualize(bodyNode);
			return _Browser_makeAnimator(initialModel, function(model)
			{
        const viewStart = performance.now();
				_VirtualDom_divertHrefToApp = divertHrefToApp;
				var doc = view(model);
				var nextNode = _VirtualDom_node('body')(_List_Nil)(doc.body);
				var patches = _VirtualDom_diff(currNode, nextNode);
				bodyNode = _VirtualDom_applyPatches(bodyNode, currNode, patches, sendToApp);
				currNode = nextNode;
				_VirtualDom_divertHrefToApp = 0;
				(title !== doc.title) && (_VirtualDom_doc.title = title = doc.title);
        const viewEnd = performance.now();
        window.perfTimes.push({ view: viewEnd - viewStart })
      });
		}
	);
});
