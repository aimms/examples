Screw.Unit(function() {with(Screw.Specifications) {with(Screw.Matchers) {

	var Addon = {
		setUp: function() {
			var self = this;

			self.sandbox = $('<div class="test-addon-palette-switch-sandbox"></div>');
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
		}
	};

	describe("palette switch addon (integration)", function() {
		var applicationElQ,
				addonElQ,
				runtimePath;

		before(function() {
			Addon.setUp();
			applicationElQ = Addon.getApplicationElQ();
			addonElQ = applicationElQ.find('.paletteswitch');
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
			assertThat(Addon.getApplicationElQ().data('aimms_palette_switch_addon'), instanceOf($.Widget));
		});

		describe("after palette 'Colorbrewer 11 color qualitative ' is selected", function() {

			before(function() {
				addonElQ.find('select').val('colorbrewer-qualitative-11');
				addonElQ.find('select').change();
			});

			it("should have set the correct class", function() {
				assertThat($('html').hasClass('palette-colorbrewer-qualitative-11'));
			});

			describe("after the default palette is selected", function() {

				before(function() {
					addonElQ.find('select').val('default');
					addonElQ.find('select').change();
				});

				it("should have updated the palette css path", function() {
					assertThat(!$('html').hasClass('palette-colorbrewer-qualitative-11'));
				});

			});

		});

	});

}}});

