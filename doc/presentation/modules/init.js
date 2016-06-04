var aeListener = {};

function disableAeListeners() {
	for(var i in aeListener) {
		aeListener[i].disable();
	}
}

function enableAeListener(listener) {
	aeListener[listener].enable();
}