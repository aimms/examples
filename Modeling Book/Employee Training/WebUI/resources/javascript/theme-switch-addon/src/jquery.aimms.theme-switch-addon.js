(function($, AWF){

	var logId = 'addons.theme-switch',
			log = log4javascript.getLogger(logId);

	//setupLogger(logId, "TRACE");


	$.widget('aimms.aimms_theme_switch_addon', AWF.Widget.create({
		_create: function() {
			var widget = this;
			widget.currTheme = '';

			widget.themeswitcher = $('<div class="themeswitcher">\
				<select>\
					<option value="default">Default theme</option>\
					<option value="dark">Dark theme</option>\
					<option value="green">Green theme</option>\
				</select>\
			</div>');

			widget.themeswitcher.find('select').on('change', function(e) {
				var theme = $(this).val();
				widget._changeTheme(theme);
			});

			widget.element.prepend(widget.themeswitcher);
		},
		_changeTheme: function(theme) {
			var widget = this,
					documentClass = void 0,
					documentElQ = $('html');

			if (theme !== 'default') {
				documentElQ.removeClass(widget.currTheme);
				documentClass = widget.currTheme = 'theme-' + theme;
				documentElQ.addClass(documentClass);
			} else {
				documentElQ.removeClass(widget.currTheme);
			}

			widget.element.awf.specifiedOptions('themes.current', AWF.OptionUtil.createSpecifiedOption(theme), {fromUI: true});
		},
		onResolvedOptionChanged: function(option, value) {
			if (option === 'themes.current' && value) {
				var widget = this;
				widget._changeTheme(value);
				widget.themeswitcher.find('select').val(value);
			}
		}
	}));

})(jQuery, AWF);
