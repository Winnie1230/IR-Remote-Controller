var WebSocket_Host = "ws://winnieliu.ddns.net:8765"
var port = 11230
/*-----Create js Websocket-----*/
//var ws = new WebSocket("ws://nckuwinnieliu.ddns.net:8765");

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
            if(evt.data == "timeout"){
	    	    $.alert({
                    theme: 'modern',
                    icon: 'fa fa-warning',
                    columnClass: 'col-md-5 col-md-offset-5',
                    //columnClass: 'large',
                    closeIcon: true,
                    type: 'red',
                    typeAnimated: true,
 
                    title: 'Alert!',
                    content: 'Time Out!!! Please Search Product Number again.',
                });
                $("#remind_text").text("Time Out!!! Please enter Product Number again.");
            }
		    else{
	    	    var received_msg = jQuery.parseJSON(evt.data);
                //console.log(jQuery.type(received_msg));
        	    console.log(received_msg);
 			    $("#remind_text").text("Data Loading Finish!!!");
                SetState(received_msg);

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
            theme: 'modern',
            icon: 'fa fa-warning',
            //columnClass: 'large',
            closeIcon: true,
            type: 'red',
            //typeAnimated: true,
 
            title: "Alert!",
            content: "Enter Product Number!!!",
        })
    }
 });

function SetState(received_msg){
    var sensor_tag = ["#co2_state","#pm2.5_state","#pm10_state","#hcho_state","#tvoc_state","#humid_state","#temp_state","#current_state"];
    var sensor = ["co2","pm2.5","pm10","hcho","tvoc","humid","temp","current"];

    for(var i = 0; i < 8; i++){
        var state = received_msg[sensor[i]]; //received state
        var tag = sensor_tag[i]; //sensor_tag
        /*-----set sensor state-----*/
        if(state == "on"){
            if($(tag).text() == "OFF"){
                $(tag).text("ON");
                $(tag).removeClass("badge-danger");
                $(tag).addClass("badge-success");
            }
        }
        else{ //sensor_state == off
            if($(tag).text() == "ON"){
                $(tag).text("OFF");
                $(tag).removeClass("badge-success");
                $(tag).addClass("badge-danger");
            }
        }
    }
}

function ReloadFunc(){
    console.log(product_num);
	//$("#remind_text").text("Reload States!!!");

    /*-----Create js Websocket-----*/
    var ws = new WebSocket("ws://winnieliu.ddns.net:8765");
    ws.onopen = function(){
        console.log("connected");
    }
    ws.onmessage = function(evt){
        if(evt.data == "timeout"){
	    	$.alert({
                theme: 'modern',
                icon: 'fa fa-warning',
                columnClass: 'col-md-5 col-md-offset-4',
                closeIcon: true,
                title: 'Alert! Reload Time Out!!!',
                content: 'Please Press Reload Button to Reload States.<br>',
                type: 'red',
                typeAnimated: true,
            });
            $("#remind_text").text("Time Out!!! Please Reload again.");
        }
		else{
	    	var received_msg = jQuery.parseJSON(evt.data);
        	//console.log(jQuery.type(received_msg));
        	console.log(received_msg);
 			
            $("#remind_text").text("Data Loading Finish!!!");
			SetState(received_msg);
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

}

$("#reload_button").click(()=>{
    $("#remind_text").text("Reload States!!!");
    ReloadFunc();
});

function SetDisable(){
    var sensor_tag = ["#co2_state","#pm2.5_state","#pm10_state","#hcho_state","#tvoc_state","#humid_state","#temp_state","#current_state"];

    for(var i = 0; i < 8; i++){
        var tag = sensor_tag[i]; //sensor_tag
        /*-----set sensor state-----*/
        if($(tag).text() == "ON"){
            $(tag).text("OFF");
            $(tag).removeClass("badge-success");
            $(tag).addClass("badge-danger");
        }
    }

	$("#reload_button").prop('disabled',true);
	$("#sensor_list").prop('disabled',true);
	$("#on_off_list").prop('disabled',true);
	$("#send_button").prop('disabled',true);
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
    $("#remind_text").text("Data Sent. Reloading States.");

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
                    ReloadFunc();
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
        $("#sensor_list").prop('selectedIndex',0);
        $("#on_off_list").prop('selectedIndex',0);
	}
})

$("#logout_button").click(()=>{
    $.confirm({
        icon: 'fas fa-sign-out-alt',
        theme: 'material',
        title: 'Confirm LogOut!',
        content: 'Are you sure to logout?',
        type: 'dark',
        typeAnimated: true,
        buttons: {
            tryAgain: {
                text: 'Logout',
                btnClass: 'btn-dark',
                action: function(){
			        window.location.href = "/";
                }
            },
            cancel: function () {
            }
        }
    });
});
