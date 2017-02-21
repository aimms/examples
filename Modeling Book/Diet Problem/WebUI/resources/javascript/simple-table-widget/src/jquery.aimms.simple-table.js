(function($) {

var log = log4javascript.getLogger("widgets.simple-table");
//setupLogger("widgets.simple-table2", "TRACE");

/**
 *	An AWF Widget or Widget Addon is a 'subclass' (i.e. derived from a protype)
 *	of a jQuery UI widget. Most, if not all, documentation of jQuery-UI framework¹
 *	is also applicable here.
 *
 *	____
 *	¹ http://www.erichynds.com/jquery/tips-for-developing-jquery-ui-widgets/
 */
var SimpleTableWidget = AWF.Widget.create({

	/**
	 * This is the standard jQuery-UI create function. Please refer to the
	 * jQuery-UI documentation for more information.
	 */
	_create: function _create() {
		var widget = this;

		widget.simpleTableWrap = $('<div class="simpleTableWrap"></div>');
		widget.tableElQ = $('<table>');

		widget.simpleTableWrap
			.append(widget.tableElQ);

		widget.element.find('.awf-dock.center')
			.append(widget.simpleTableWrap);
	},

/*
 *
 *	Constructs a table, installs listeners and attempts to fill the table.
 *	The attempt to fill the table also triggers the asynchronous data retrieval which,
 *	when the data becomes available, will notify the installed listeners.
 *
 * @param {type} partDataSources, an object containing the following properties:
 *
 *		rowHeader		- The part datasource that contains the row header data
 *		colHeader		- The part datasource that contains the column header data
 *		grid			- The part data source that contains the values that are
 *						  spanned by the colHeaderPart and the rowHeaderPart.
 *


                                   The Anatomy of a Table
                                   ======================


                                             .-------- colHeader.getNumRows()  <<-- CAVEAT LECTOR (transposed)
                                            /
                                  |-----------------------|

                             +-----+-----------------------+ ---
                             |Pivot|                       |  |
                             |Area |       colHeader       |  | ------ colHeader.getNumCols()  <<-- CAVEAT LECTOR (transposed)
                             |     |                       |  |
                        ---  +-----x-----------------------+ ===
                         |   |  r  |                       |  |
  rowHeader.getNumRows() |   |  o  |                       |  |
                  \      |   |  w  |                       |  |
                   `-----|   |  H  |                       |  |
                         |   |  e  |         grid          |  | ------ grid.getNumRows()
                         |   |  a  |                       |  |
                         |   |  d  |                       |  |
                         |   |  e  |                       |  |
                         |   |  r  |                       |  |
                        ---  +-----+-----------------------+ ---                                  x: 'the pivot point'

                             |-----|-----------------------|
                               /               \
  rowHeader.getNumCols() -----'                 `------- grid.getNumCols()

 *
 *
 */
	_constructTable: function _constructTable(dataSource) {
		var widget = this;
		var rowHeader = dataSource.rowHeader;
		var colHeader = dataSource.colHeader;
		var grid = dataSource.grid;

		log.debug("rowHeader.getNumRows(): ", rowHeader.getNumRows());
		log.debug("rowHeader.getNumCols(): ", rowHeader.getNumCols());

		log.debug("rowHeader.getNumRows(): ", colHeader.getNumRows());
		log.debug("rowHeader.getNumCols(): ", colHeader.getNumCols());

		log.debug("grid.getNumRows(): ", grid.getNumRows());
		log.debug("grid.getNumCols(): ", grid.getNumCols());

		function updateTableCell(type, row, col, text) {
			widget.tableElQ.find('.'+type+'.row'+row+'.col'+col).text(text);
		}

		// 1. Create the table.
		/**
		 *
		 * createTdOrTh('th', 'foo', 3, 4)  ==>  '<th class="foo row3 col4"></th>'
		 *
		 */
		function createCellElQ(type, name, row, col) {
			return $(['<', type, ' class="', name, ' row'+row, ' col'+col, '">X</', type, '>'].join(''));
		}

		// 1a. Create the column header
		var theadElQ = $('<thead>');
		colHeader.getNumCols().times(function(col) {
			var trElQ = $('<tr>');
			colHeader.getNumRows().times(function(row) {
				trElQ.append(createCellElQ('th', 'colHeader', row, col));
			});
			theadElQ.append(trElQ);
		});

		// 1b. Add the pivot area
		theadElQ.find('tr:first').prepend('<th colspan="'+rowHeader.getNumCols()+'" rowspan="'+colHeader.getNumCols()+'"></th>');

		// 1c. Create the row header and grid
		var tbodyElQ = $('<tbody>');
		rowHeader.getNumRows().times(function(row) {
			var trElQ = $('<tr>');
			rowHeader.getNumCols().times(function(col) {
				trElQ.append(createCellElQ('th', 'rowHeader', row, col));
			});
			grid.getNumCols().times(function(col) {
				trElQ.append(createCellElQ('td', 'grid', row, col));
			});
			tbodyElQ.append(trElQ);
		});

		// 1d. Construct the table
		widget.tableElQ.empty();
		widget.tableElQ.append(theadElQ);
		widget.tableElQ.append(tbodyElQ);

		// 2. Fill the table  (uses asynchronous data retrieval)
		dataSource.requestDataBlocks(
			[
				{start: 0, end: grid.getNumRows()},
				{start: 0, end: grid.getNumCols()},
			],
			["values"],
			function onReady(layeredDataBlocks) {
				['rowHeader', 'colHeader', 'grid'].forEach(function(type) {
					var partDataSource = dataSource[type];
					partDataSource.getNumRows().times(function(row) {
						partDataSource.getNumCols().times(function(col) {
							updateTableCell(type, row, col, layeredDataBlocks[type].getLayer("values").get(row, col));
						});
					});
				});
			}
		);
	},

	/**
	 * The most important message on the per-widget message bus is the 'resolvedOptionChanged' message.
	 * It has two arguments:
	 *
	 *	1) optionName		- The option name that was specified in the factory
	 *	2) value			- The value for the option. It can be 'null', which, if it is,
	 *						  means 'go back to the default value'. If it is non-null, the
	 *						  value is guaranteed to be of the type that the factory specified
	 *						  for the option.
	 */
	onResolvedOptionChanged: function(optionName, value) {
		var widget = this;

		if(optionName === "contents") {
			if(value) {
				// 'value' is the datasource (because in the factory we specified the 'contents' option to be a datasource)
				// 'rows' and 'cols' arethe part names that we specified in the OptionTypeTable
				// 'table' because we want to access it as a table and not as a tree (value.list.tree); the extra '.table'
				// was a bad and obsolete idea, it is scheduled to be removed;
				widget._constructTable(value);
			} else {
				widget.tableElQ.empty();
			}
		}
	},
});
// The jQuery-UI way of registering/creating a new widget:
$.widget('ui.aimms_simple_table', SimpleTableWidget);

})(jQuery);
