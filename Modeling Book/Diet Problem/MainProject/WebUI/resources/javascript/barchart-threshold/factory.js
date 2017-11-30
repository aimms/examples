(function($) {

// for DRY-ness
var widgetType = "barchart";

AWF.installListenerToMethodBridges(AWF, {
	postDecorateElement: function(elQ, type) {
		if (type === widgetType) {
			elQ.aimms_barchartthreshold_addon();
		}
	},
});

})();
