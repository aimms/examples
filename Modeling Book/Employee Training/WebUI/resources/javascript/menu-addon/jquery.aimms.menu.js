(function($) {

var logId = "addons.menu_addon";
	log = log4javascript.getLogger(logId);
//setupLogger(logId, "TRACE");

var menuAddon = AWF.Widget.create({

	_create: function _create() {
		var addon = this;
		addon.navMenuElQ = $('<ul class="menubar"></ul>');

		addon.pages = {};
		addon.element.append(addon.navMenuElQ);
	},

	onParsedOptionChanged: function(optionName, pagesList, context) {
		var addon = this;
		if (optionName == "pages") {
			addon.refreshMenu(pagesList || []);
		}
	},
	afterResolvedOptionChanged: function(name, pageUri) {
		var addon = this;

		if (name === "pages.current" && pageUri) {
			var searchString = 'a[data-uri="' + pageUri + '"]';

			addon.navMenuElQ.find(".active, .ancestor").removeClass("active").removeClass("ancestor");

			addon.navMenuElQ.find(searchString)
				.parent().addClass("active")
				.parents("li").addClass("ancestor");
		}
	},

	refreshMenu: function(pageslist) {
		var addon = this;
		var currentpage = addon.element.awf.resolvedOptions("pages.current") || "";
		var topLevels = [];
		pageslist = _.sortBy(pageslist, "uri");

		addon.navMenuElQ
			.empty()
			.append('<li><a data-uri="home" data-link="home"><span>home</span></a></li>');


		pageslist.forEach(function (page) {
			if (page.uri.includes("/")) {
				var ancestor = page.uri.split("/")[0];
				topLevels.push({uri: ancestor, children:[]});
			} else {
				topLevels.push(page);
			}
		});

		_.uniq(topLevels, function(n) {return n.uri})
			.forEach(function (page) {
				pageslist.filter(function(subpage) {
					return subpage.uri.startsWith(page.uri + "/");
				}).forEach(function (subpage) {
					if(!page.children) {
						page.children = [];
					}
					page.children.push(subpage);
				});

				var item = $("<li>");

				if(page.children) {
					item
						.append('<a data-uri="' + page.uri + '"><span>' + page.uri + '<i class="icon-arrow-down2"></span></i></a>')
						.append("<ul>");

					_.uniq(page.children, function(n) {return n.uri}).forEach(function (subpage) {
						var newSubpage = subpage.uri.split("/");
						newSubpage.shift();
						newSubpage = newSubpage.join("/");

						item.find("ul").append('<li><a data-uri="' + subpage.uri + '" data-link="'+subpage.uri+'"><span>' + newSubpage + '</span></a></li>');
					})
				} else {
					item.append('<a data-uri="' + page.uri + '" data-link="'+page.uri+'"><span>' + page.uri + '</span></a>');
				}

				addon.navMenuElQ.append(item);
			});

			addon.navMenuElQ.find("a").on("click", function (e) {
				e.preventDefault();
				AWF.Bus.requestPageChange($(this).attr("data-link"));
				return false;
			});
	},
});

$.widget("aimms.aimms_menu_addon", menuAddon);

})(jQuery);
