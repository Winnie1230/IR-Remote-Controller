var host = "nckuwinnieliu.ddns.net"
var port = 11230
/*-----Create js Websocket-----*/
var ws = new WebSocket("ws://nckuwinnieliu.ddns.net:8765");

$("#send_button").click(()=>{
    send_topic = $("#input_topic").val();
    send_message = $("#input_message").val();
    
    /*clear input bar*/
    $("#input_topic").val("");	
    $("#input_message").val("");

    /*show on receive block*/
    $("#mqtt_receive_topic").append('<p class="receive_message_content">'+send_topic+'</p>');
    $("#mqtt_receive_message").append('<p class="receive_message_content">'+send_message+'</p>');

    $.ajax({
	url: "/send?"+"topic="+send_topic+"&message="+send_message,
	type: 'GET',
	data: {
		//user_name: $('#user_name').val()
	},
	error: function(xhr) {
		alert('Ajax request 發生錯誤');
	},
	success: function(response) {
		console.log("sucess");
	}
    });
});

$("#logout_button").click(()=>{
	alert("logout");
	window.location.href = "/";
});
