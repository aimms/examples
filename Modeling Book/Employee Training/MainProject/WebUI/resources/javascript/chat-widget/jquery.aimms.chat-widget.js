// jquery.aimms.chat-widget.js
jQuery.widget('ui.aimms_chat_widget', AWF.Widget.create({
	_create: function() {
		var widget = this;
		widget.contentElQ = $('<iframe class="chat" src="http://tlk.io/demo-chatbox"></iframe>');

		widget.element
				.find('.awf-dock.center')
				.append(widget.contentElQ);
		$(".chat").parent().parent().parent().find(".top").css("display","none");
	},
}));
