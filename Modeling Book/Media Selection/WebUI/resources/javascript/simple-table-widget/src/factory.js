(function($) {

/**
 *  In AWF, the AIMMS Widget Framework, a DOM tree element can be instantiated as an aimms-widget.
 *
 *  The selected DOM tree element will then be decorated (in a process similar to dependency injection)
 *  by various components. These components interact with each other without directly referencing one
 *  another. Rather each component has a 'contract' with the bus: A set of messages that will send on
 *  the bus when certain other messages are sent.
 *
 *  Components can be:
 *
 *  1) Widgets
 *  2) Widget Addons
 *  3) Plain JavaScript Objects that interact with the bus directly.
 *
 *  For simplicity's sake, we will focus on these first two here.
 *
 *  A Widget (sometimes also call Control) 'displays an information arrangement changeable by the user'¹.
 *  Typically this is something like a chart, a table, a dropdown, a button, etc.
 *  Examples of a widget:
 *
 *		* widgets/label				- very basic, only a single scalar option value that is directly available
 *		* widgets/simple-select		- (this widget) a more comprehensive widget that uses datasources to
 *									  retrieve data asynchronously.
 *
 *  A Widget Addon is a piece of extra functionality that can generically apply to more than one specific
 *  widget. (For instance, a 'typeswitch addon' that overlays the current widget with possible types to
 *  switch to, is applicable to multiple widgets.)
 *  Examples of a widget-addon:
 *
 *		* widgets/typeswitch-dropdown-addon		- Adds a typeswitch dropdown to the toolbar²
 *		* widgets/generic-pivot-addon			- Adds generic pivot ui for widgets that don't have a pivot ui themselves²
 *
 *  _______
 *	¹ http://en.wikipedia.org/wiki/GUI_widget
 *	² Don't forget to use F4 first to switch to edit mode; these addons are not visible during normal view mode anymore.
 *
 *
 *
 *	Both widgets and widget addons are usually implemented as one or more jQuery (UI) plugins.
 *	The implementation is broken up into several parts (and files):
 *
 *	1) <widget-name>/factory.js   - (file your are reading now), a file that is responsible for interacting with the AWF
 *									bus in order to properly instantiate a widget (and invoke a jQuery plugin on the DOM
 *									tree element that is going to be the AWF widget).
 *	2) <widget-name>/jquery.xy.js - a file that implements a jQuery (UI) plugin that implements the (graphical) behavior
 *	3) <widget-name>/jquery.xy.css- a css file with widget specific (behavioral) css
 *
 *	Sometimes, for complex widgets, the jQuery plugin is split into several plugins, you'll see:
 *
 *	1) <widget-name>/factory.js
 *	2) <widget-name>/jquery.xy.js							  implements a pure jquery plugin that has nothing to do with AWF
 *															  (could also have use as a seperate plugin)
 *	3) <widget-name>/jquery.xy.controller.js				  acts as the controller between the per-widget bus and the jquery
 *															  plugin above.
 *	4) <widget-name>/foo-support/jquery.xy.foo-support.js     (implements foo-support for the widget. The widget will operate
 *															   correctly if foo-support is not loaded (decoupling), but it is
 *															   not implemented as a widget-addon, because it is too specific
 *															   for this widget)
 *
 *	etc.
 *
 *	Files are loaded by AWF by then adding the above files to web-gui-includes.js. AWF will then make sure they are loaded
 *	at the right moment.
 *
 *	Note: Manually including files using <script> tags will not work. It is also undesirable, because you only want to specify the
 *	list of to be loaded files once and not in each static .html file (and you also want to reuse the include list when running
 *	the tests).
 */

/*************************************
 *		Widget and Option Types      *
 *************************************/

/**
 * A widget or add-on can have one or more options. An option has a name and a type.
 *
 * Which options a widget has, is registered during the handling of the "initializeOptionTypes" message
 * (see below in section 'Message Handling').
 *
 * Basic option types are 'string' and 'integer', but also more encompassing types such as 'function', or
 * (the most complex option type thusfar): {type: "datasource", ...}.
 *
 * (Please note that 'string', 'integer', and 'function' are specified as a string literal; the datasource
 *  option type is specified as an object, because it has to unveil more specific information about the datasource)
 *
 * The object below declares a datasource option type with the following characteristics:
 *
 * * The datasource uses a caching mechanism, indicating that this widget can handle situation where
 *   the data set is very large and not all data has to be retrieved at once.
 * * The multi-dimensional data set can be partitioned into two parts: 'rows' and 'cols'. The part should be of type
 *   table (currently the only part type directly supported by our data provisioning).
 * * The default partitioning (the process of assigning dimensions to bins automatically, for example
 *   when there is no partitioning explicitly specified by the user).
 */
var OptionTypeTable = {
						type : "datasource",
					    parts : [ {name : "rowHeader"}, {name : "colHeader"} ],
						alternativeNameForValuesProperty: "grid",
					    preferredPartitionSizes : [	{parts: ["rowHeader","colHeader"]}, {parts: "aggregated"} ]
				      };


// for DRY-ness
var widgetType = 'simple-table';


/******************************
 *		Message Handling      *
 ******************************/

/**
 *	In AWF there are two bus types: The global AWF bus and the per-widget bus (for local widget related messaging).
 *
 *	Here this factory installs a few listeners on the global AWF bus. On this bus widget instantiations are
 *	being negotiated and announced, giving factories a change to interact with the AWF widget instantiation process.
 *
 *	Once a widget has been instantiated, it will have its own per-widget bus that can be used for the local
 *	messaging mentioned above.
 */
AWF.installListenerToMethodBridges(AWF, {
	/**
	 * The collectTypes message allows this widget the register itself in a list of known widget types.
	 * If this widget type is registered, then it can be switched to when a user changes widget types.
	 * Whether or not a user can switch to this type depends on the context (for example, it is not always
	 * logical to be able to switch from e.g. a charting widget to e.g. a button).
	 *
	 * Several components (whom this widget does not know directly) can send this message. Whether this widget
	 * adds itself to the collectedTypes depends on the context, for which the "contextElQ" can be used.
	 *
	 * The contextElQ, if specified, denotes the DOM tree element that is going to be the widget element, should
	 * there be a type switch at some point during the lifecycle of the program. As such, the awf.tags can be
	 * inspected to determine whether or not this factory deems it possible to switch to the type.
	 *
	 * If contextElQ is not specified, this factory is expected to register its widget type (i.e. add the type
	 * to collectedTypes).
	 */
	onCollectTypes: function(collectedTypes, contextElQ) {
		if(!contextElQ || contextElQ.awf.tags("contents property") || contextElQ.awf.tags("placeable")) {
			collectedTypes.push(widgetType);
		}
	},

	/**
	 * To maximize reuse, decoupling and cohesion. The complete functionality of an instantiated widget is
	 * not determined by a single responsible entity. Instead, DOM tree elements are decorated by seperate
	 * pieces of functionality (Widgets and Widget-Addons), each strictly separated from another (decoupling),
	 * with a specific purpose/functionality (cohesion).
	 *
	 * But when there is not a direct reference to other widget types or addons, there must be a mechanism to
	 * determine when to add the specific functionality to the DOM tree element on which an AWF widget is being
	 * instantiated. This is done through the exchange of "tags". For instance, the generic pivot addon will
	 * contribute itself when the current widget type declares that it has a "pivotable contents property":
	 *
	 * Excerpt (transcribed) from generic-pivot-addon/factory.js:
	 * <code>
	 * onInitializeTags: function(elQ, type) {
         if(elQ.awf.tags("pivotable contents property")) {
           elQ.awf.tags(["pivoting ui"], 'add'); // i.e. "if the element receives a tag with 'a pivotable contents property',
                                                 //       then this factory declares that it will provide a 'pivoting ui' for it"
         }
       }
	 * </code>
	 *
	 * (Please note that the list of tags is still somewhat in flux.)
	 *
	 * (Please also note that this mechanism allows an extreme form of dependency injection with "negotatiation" between
	 * decoupled entities.)
	 */
	onInitializeTags: function(elQ, type) {
		if(type === widgetType) {
			elQ.awf.tags(["complex widget", "contents property", "simple-table", "table-widget"], 'add');
		}
	},

	/**
	 * The initializeOptionTypes message allows this factory to declare which option this widget type has, and, what the option
	 * types for these options are.
	 *
	 * Here, we specify that this widget has an option value named 'contents', which is of type 'OptionTypeSelect'; please
	 * refer the the section 'Widget and Option Types' above for more details about the specifics of this option type.
	 */
	onInitializeOptionTypes: function(elQ, type) {
		if(type === widgetType) {
			AWF.OptionTypeCollector.addOptionType(elQ, "contents", OptionTypeTable);
		}
	},

	/**
	 * When a widget of a specific type is going to be instantiated, the decorateElement message is used to inform all
	 * observers of the AWF bus that for the given DOM tree element (as a jQuery object), AWF is going to instantiate
	 * a widget of the given type. This message gives the factories of all widgets and widget-addons to decorate the
	 * element with their functionality.
	 */
	onDecorateElement: function(elQ, type) {
		if(type === widgetType) {
			elQ.aimms_simple_table();
		}
	},
});

})(jQuery);
