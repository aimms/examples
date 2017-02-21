// factory.js
AWF.Bus.subscribe({
	onDecorateElement: function(elQ, type) {
		if (type === "chat") {
			elQ.aimms_chat_widget();
		}
	},
		onCollectTypes: function(collectedTypes, contextElQ) {
		if (!contextElQ || contextElQ.awf.tags("placeable")) {
			collectedTypes.push("chat");
		}
	},
});
