var host = "winnieliu.ddns.net"
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

var product_num = ""

$("#search_button").click(()=>{
    product_num = $("#input_num").val();
    console.log(product_num);
    
    if(product_num){
        /*-----Create js Websocket-----*/
        var ws = new WebSocket("ws://winnieliu.ddns.net:8765");
		var result = -1;

        ws.onopen = function(){
            console.log("connected");
			if(result == 1 || result == 2){
				ws.close();
				console.log(ws.readyState);
			}
        }
        ws.onmessage = function(evt){
            if(evt.data == "timeout")
	    	    $("#remind_text").text("Time Out!!! Please enter Product Number again.");
		    else{
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
		
			    $("#reload_button").prop('disabled',false);
		    	$("#sensor_list").prop('disabled',false);
			    $("#on_off_list").prop('disabled',false);
			    $("#send_button").prop('disabled',false);
	        }
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
				result = 0;
	        }
	        else if(response == "not_used"){
			    $("#remind_text").text("Product is not being used.");
			    SetDisable();
				result = 1;
	        }
	        else if(response == "no"){
			    $("#remind_text").text("No product number");
			    SetDisable();
				result = 2;
	        }
	    }
        });
		
		ws.onclose = function(evt){
	        console.log("Disconnect");
        }
    }
    else{
        $.alert({
            title: "Alert!",
            content: "Enter Product Number!",
        })
    }
 });

$("#reload_button").click(()=>{
    console.log(product_num);
	$("#remind_text").text("Reloading!!!");

    /*-----Create js Websocket-----*/
    var ws = new WebSocket("ws://nckuwinnieliu.ddns.net:8765");
    ws.onopen = function(){
        console.log("connected");
    }
    ws.onmessage = function(evt){
        if(evt.data == "timeout")
	    	$("#remind_text").text("Time Out!!! Please Reload again.");
		else{
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
	    }
    }

    ws.onclose = function(evt){
	console.log("Disconnect");
    }
		
    $.ajax({
	url: "/reload?"+"product="+product_num,
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

	$("#reload_button").prop('disabled',true);
	$("#sensor_list").prop('disabled',true);
	$("#on_off_list").prop('disabled',true);
	$("send_button").prop('disabled',true);
}

$("#sensor_list").change(()=>{
    console.log("change");
    $("#sensor_list").blur();
})

$("#sensor_list").focus(()=>{
    $("#sensor_list").val('');
})

$("#on_off_list").change(()=>{
    $("#on_off_list").blur();
})

$("#on_off_list").focus(()=>{
    $("#on_off_list").val('');
})

$("#send_button").click(()=>{
    send_topic = $("#sensor_list").val();
    send_message = $("#on_off_list").val();

	if(send_topic && send_message){
		$.ajax({
				url: "/send?"+"product="+product_num+"&topic="+send_topic+"&message="+send_message,
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

        /*$.confirm({
			title: "Confirm!",
			content: "Sure to change sensor state?",
			buttons:{
                confirm: ()=>{
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
				},
				
                cancel: ()=>{
				    //$.alert('Canceled');
			    },
			}
	    });*/
        
        //clear input bar
		$("#sensor_list").prop('selectedIndex',0); 
    	$("#on_off_list").prop('selectedIndex',0);
    }
	else{
		$.alert({
			title: "Alert!",
			content: "Please choose sensor and its state!",
		});
	}
})

$("#logout_button").click(()=>{
   $.confirm({
    title: 'Confirm!',
    content: 'LogOut confirm!',
    buttons: {
        confirm: ()=>{
			window.location.href = "/";
		},
        cancel: ()=>{},
    }
}); 
});
