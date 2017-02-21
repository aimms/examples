(function($, AWF){

	var logId = 'addons.palette-switch',
			log = log4javascript.getLogger(logId);
	//setupLogger(logId, "TRACE");

	var PALETTES = [
		{name: "Default Palette", className: "default"},
		{name: "ColorBrewer 11", className: "colorbrewer-qualitative-11"},
		{name: "ColorBrewer 7",  className: "colorbrewer-qualitative-7"},
	];

	$.widget('aimms.aimms_palette_switch_addon', AWF.Widget.create({
		_create: function() {
			var widget = this;
			var options = '';
			widget.currpalette = '';

			widget.paletteswitcher = $('<div class="paletteswitch">'
				+ '<select></select>'
				+ '</div>');

			PALETTES.forEach(function(palette) {
				options += '<option value="' + AWF.I18n.get(palette.className) + '">' + palette.name + '</option>';
			});

			widget.paletteswitcher.find("select").append(options);

			widget.paletteswitcher.find('select').on('change', function(e) {
				var palette = $(this).val();
				widget._changepalette(palette);
			});

			widget.element.append(widget.paletteswitcher);
		},
		_changepalette: function(palette) {
			var widget = this;
			var documentElQ = $('html');

			if (palette !== 'default') {
				documentElQ.removeClass(widget.currpalette);
				widget.currpalette = 'palette-' + palette;
				documentElQ.addClass(widget.currpalette);
			} else {
				documentElQ.removeClass(widget.currpalette);
			}

			widget.element.awf.specifiedOptions('palette.current', AWF.OptionUtil.createSpecifiedOption(palette), {fromUI: true});
		},
		onParsedOptionChanged: function(option, value) {
			if (option === 'palette.current' && value) {
				var widget = this;
				widget._changepalette(value);
				widget.paletteswitcher.find('select').val(value);
			}
		}
	}));

})(jQuery, AWF);
