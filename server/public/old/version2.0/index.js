var host = "nckuwinnieliu.ddns.net"
var port = 11230
/*-----Create js Websocket-----*/
//var ws = new WebSocket("ws://nckuwinnieliu.ddns.net:8765");
/*
ws.onopen = function(){
	alert("connected");
}
ws.onmessage = function(evt){
	var received_msg = evt.data;
	console.log(received_msg);
}
*/

$(document).ready(()=>{
	console.log("document ready");
});

var first_loaded = 0 /*check if sensor data is first_loaded*/
$("#search_button").click(()=>{
	first_loaded = 0
	product_num = $("#input_num").val();
	console.log(product_num);

	/*-----Create js Websocket-----*/
	var ws = new WebSocket("ws://nckuwinnieliu.ddns.net:8765");
	ws.onopen = function(){
		console.log("connected");
	}
	ws.onmessage = function(evt){
		var received_msg = jQuery.parseJSON(evt.data);
		//console.log(jQuery.type(received_msg));
		console.log(received_msg);
		$("#remind_text").text("Data Loading Finish!!!");
		$("#co2_toggle").bootstrapToggle('enable');
		$("#co2_toggle").bootstrapToggle(received_msg.co2);

		$("#pm_toggle").bootstrapToggle('enable');
		$("#pm_toggle").bootstrapToggle(received_msg.pm);
	
		$("#current_toggle").bootstrapToggle('enable');
		$("#current_toggle").bootstrapToggle(received_msg.current);
	
		$("#temp_toggle").bootstrapToggle('enable');
		$("#temp_toggle").bootstrapToggle(received_msg.Temp);
		
		first_loaded = 1;
	}

	ws.onclose = function(evt){
		console.log("Disconnect");
	}

	/*-----clear input num-----*/
	$("#input_num").val("");
		
	$.ajax({
		url: "/search?"+"product="+product_num,
		type: 'GET',
		data: {
			//user_name: $('#user_name').val()
		},
		error: function(xhr) {
			alert('Ajax request 發生錯誤');
		},
		success: function(response) {
			console.log("sucess");
			console.log(response);
			
			if (response == "used"){
				$("#remind_text").text("Find Product! Loading its state...");
				SetDisable();
			}
			else if(response == "not_used"){
				$("#remind_text").text("Product is not being used.");
				SetDisable();
				ws.close();
			}
			else if(response == "no"){
				$("#remind_text").text("No product number");
				SetDisable();
				ws.close();
			}
		}
    });
});

function SetDisable(){
		$("#co2_toggle").bootstrapToggle('on');
		$("#co2_toggle").bootstrapToggle('disable');
				
		$("#pm_toggle").bootstrapToggle('on');
		$("#pm_toggle").bootstrapToggle('disable');

		$("#current_toggle").bootstrapToggle('on');
		$("#current_toggle").bootstrapToggle('disable');

		$("#temp_toggle").bootstrapToggle('on');
		$("#temp_toggle").bootstrapToggle('disable');
}

$("#co2_toggle").change(()=>{
    if(first_loaded == 1){
    	console.log("co2_change");

    	$("#co2_toggle").prop('checked') ? sensor_state="on" : sensor_state="off";

    	$.ajax({
	    url: "/togglechange?"+"sensor=co2"+"&state="+sensor_state,
            type: 'GET',
	    data: {
		    //user_name: $('#user_name').val()
	    },
	    error: function(xhr) {
		alert('Ajax request 發生錯誤');
	    },
	    success: function(response) {
		console.log(response);
	    }
        });
    }
})

$("#pm_toggle").change(()=>{
    if(first_loaded == 1){
    	console.log("pm_change");

    	$("#pm_toggle").prop('checked') ? sensor_state="on" : sensor_state="off";

    	$.ajax({
	    url: "/togglechange?"+"sensor=pm"+"&state="+sensor_state,
            type: 'GET',
	    data: {
		    //user_name: $('#user_name').val()
	    },
	    error: function(xhr) {
		alert('Ajax request 發生錯誤');
	    },
	    success: function(response) {
		console.log(response);
	    }
        });
    }

})

$("#current_toggle").change(()=>{
    if(first_loaded == 1){
    	console.log("current_change");

    	$("#current_toggle").prop('checked') ? sensor_state="on" : sensor_state="off";

    	$.ajax({
	    url: "/togglechange?"+"sensor=current"+"&state="+sensor_state,
            type: 'GET',
	    data: {
		    //user_name: $('#user_name').val()
	    },
	    error: function(xhr) {
		alert('Ajax request 發生錯誤');
	    },
	    success: function(response) {
		console.log(response);
	    }
        });
    }
})

$("#temp_toggle").change(()=>{
    if(first_loaded == 1){
    	console.log("temp_change");

    	$("#temp_toggle").prop('checked') ? sensor_state="on" : sensor_state="off";

    	$.ajax({
	    url: "/togglechange?"+"sensor=temp"+"&state="+sensor_state,
            type: 'GET',
	    data: {
		    //user_name: $('#user_name').val()
	    },
	    error: function(xhr) {
		alert('Ajax request 發生錯誤');
	    },
	    success: function(response) {
		console.log(response);
	    }
        });
    }
})

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
