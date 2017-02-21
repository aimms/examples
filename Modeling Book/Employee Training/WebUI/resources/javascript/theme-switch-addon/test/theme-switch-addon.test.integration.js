Screw.Unit(function() {with(Screw.Specifications) {with(Screw.Matchers) {

	var Addon = {
		setUp: function() {
			var self = this;

			self.sandbox = $('<div class="test-addon-theme-switch-sandbox"></div>');
			var applicationElQ = $('<header class="aimms-widget"></header>');

			applicationElQ.appendTo(self.sandbox);
			self.sandbox.prependTo($(document.body));

			AWF.requestDecorateElement(applicationElQ, 'application');

			self.getApplicationElQ = function() {
				return applicationElQ;
			}
		},

		tearDown: function() {
			this.sandbox.remove();
			$('head [id="theme"]').remove();
		}
	};

	describe("Theme switch addon (integration)", function() {
		var applicationElQ,
				addonElQ,
				runtimePath;

		before(function() {
			Addon.setUp();
			applicationElQ = Addon.getApplicationElQ();
			addonElQ = applicationElQ.find('.themeswitcher');
			runtimePath = AWF.runtimePath;
			AWF.runtimePath = 'runtimePath'
		});

		after(function() {
			if (!Screw.isStandaloneTest()) {
				Addon.tearDown();
			}
			AWF.runtimePath = runtimePath;
		});

		it("should have added the addon", function() {
			assertThat(Addon.getApplicationElQ().data('aimms_theme_switch_addon'), instanceOf($.Widget));
		});

		describe("after theme 'Dark theme' is selected", function() {

			before(function() {
				addonElQ.find('select').val('dark');
				addonElQ.find('select').change();
			});

			it("should have updated the theme css path", function() {
				expect($('head #theme').attr('href')).to(equal, AWF.runtimePath + '/resources/themes/dark/theme.css');
			});

			describe("after the default theme is selected", function() {

				before(function() {
					addonElQ.find('select').val('');
					addonElQ.find('select').change();
				});

				it("should have updated the theme css path", function() {
					expect($('head #theme').attr('href')).to(equal, '');
				});

			});

		});

		describe("after the default theme is selected", function() {

			before(function() {
				addonElQ.find('select').val('');
				addonElQ.find('select').change();
			});

			it("should not have set a theme file", function() {
				expect($('head #theme').attr('href')).to(equal, '');
			});

		});

	});

}}});