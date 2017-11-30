(function($) {

function forApplicableElQs(elQ, type, onApplicable) {
	if(elQ.awf.tags("application")) {
		onApplicable.call();
	}
}

AWF.installListenerToMethodBridges(AWF, {
	onDecorateElement: function(elQ, type) {
		forApplicableElQs(elQ, type, function() {
			elQ.aimms_menu_addon();
		});
	}
});

})(jQuery);
