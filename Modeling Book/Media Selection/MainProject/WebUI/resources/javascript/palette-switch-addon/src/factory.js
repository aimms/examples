(function(AWF){
	AWF.Bus.subscribe({
		onInitializeOptionTypes: function(elQ, type) {
			if (type === 'application') {
				AWF.OptionTypeCollector.addOptionType(elQ, 'palette.current', 'string');
			}
		},
		onDecorateElement: function(elQ, type) {
			if (type === 'application') {
				elQ.awf.bus.subscribe({
					onDecorateChromebar: function(chromebarElQ) {
						chromebarElQ.aimms_palette_switch_addon();
					}
				});
			}
		}
	});
})(AWF);
