(function($) {

var log = log4javascript.getLogger("widgets.barchartthreshold_addon");
setupLogger("widgets.barchartthreshold_addon", "WARN");

var barchartThresholdAddon = AWF.Widget.create({

	_create: function _create() {
		var barchart = this;
	},

	_render: function(values, header, grouped, stacked) {
		var barchart = this;
		var elQ = barchart.element;
		
		var chartElQ = elQ.find("svg>g");
		var chartLayer = d3.select(elQ.find("svg>g")[0]);

		var numGroups = header.getNumRows();
		var numBarsPerGroup = grouped.getNumRows();
		var numStacked = stacked.getNumRows();

		// compute minimal and maximal value along the y-axis
		var minValue = Number.POSITIVE_INFINITY;
		var maxValue = Number.NEGATIVE_INFINITY;
		var valuesLayer = values.getLayer("values");
		for (var l = 0; l < numGroups; l++) {
			for (var g = 0; g < numBarsPerGroup; g++) {
				if (numStacked > 0) {
					var sumPos = 0;
					var sumNeg = 0;
					for (var s = 0; s < numStacked; s++) {
						var value = valuesLayer.get(l, g, s);
						if (value >= 0) {
							sumPos += value;
						} else {
							sumNeg += value;
						}
					}
					if (sumPos > maxValue)
						maxValue = sumPos;
					if (sumNeg < minValue)
						minValue = sumNeg;
				}
			}
		}

		// make sure 0 is always being displayed
		if ( minValue > 0 ) minValue = 0;
		if ( maxValue < 0 ) maxValue = 0;

		// store minValue and maxValue
		barchart.minValue = minValue;
		barchart.maxValue = maxValue = maxValue + 10; // @TODO make this a logical value such as next whole number

		var graphWidth = chartElQ.find(".hline").first().attr("x2");
		var graphHeight = -1 * chartElQ.find(".vline").first().attr("y2");
		
		// determine ticks to be displayed along y-axis
				// Set up the y scale of the chart.
				var y = d3.scale
					.linear()
					.domain([minValue, maxValue])
					.range([0, graphHeight]);

		// assume absolute values
		var lowerbound = barchart.element.awf.resolvedOptions("contents.threshold.low");
		var upperbound = barchart.element.awf.resolvedOptions("contents.threshold.high");

		if(upperbound) {
			chartElQ.find(".threshold-high").hide().remove();
			chartLayer.append("svg:line")
				.attr("class", "hline threshold-high")
				.attr("x1", 0)
				.attr("x2", graphWidth)
				.attr("y1", y(upperbound))
				.attr("y2", y(upperbound))
			;
		}

			if(lowerbound) {
			chartElQ.find(".threshold-low").hide().remove();
			chartLayer.append("svg:line")
				.attr("class", "hline threshold-low")
				.attr("x1", 0)
				.attr("x2", graphWidth)
				.attr("y1", y(lowerbound))
				.attr("y2", y(lowerbound))
			;
		}
	},

	afterResolvedOptionChanged: function(optionName, value) {
		var barchart = this;
		var elQ = barchart.element;

		if (optionName === "contents") {
			if(value) {
				var ranges = value.values.dimension.times(function(dim){return {start:0, end: value.values.size(dim)}});
				value.requestDataBlocks(
					ranges,
					["values", "annotations"],
					function onReady(layeredDataBlocks) {
						barchart._render(
							//					 part info			part data
							$.extend({}, value.values,	layeredDataBlocks.values),
							$.extend({}, value.header,	layeredDataBlocks.header),
							$.extend({}, value.grouped, layeredDataBlocks.grouped),
							$.extend({}, value.stacked, layeredDataBlocks.stacked)
						);
					}
				);
			}
		} else if (optionName === "contents.threshold.high" || optionName === "contents.threshold.low") {
		}
	},
});
$.widget('aimms.barchartthreshold_addon', barchartThresholdAddon);

})(jQuery);
