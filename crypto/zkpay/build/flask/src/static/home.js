function formatTime(timestamp){
    let dt = new Date(timestamp * 1000);
    let time = dt.getFullYear() + "/" + (dt.getMonth()+1) + "/" + dt.getDate() + " " + dt.getHours() + ":" + dt.getMinutes();
    return time;
}

function getAccountData()
{
    $.get( "/api/accountData", function( data ) {
	$("#balance").text(data["balance"]);
	$("#flag").text(data["flag"]);
	let tBody = $("#trx").find('tbody');
	tBody.empty();
	for(i=0; i<data["trxList"].length; ++i){
	    let tr = $("<tr>");
	    let td = $("<td>");
	    td.text(formatTime(data["trxList"][i][1]));
	    tr.append(td);
	    td = $("<td>");
	    let a = $("<a>")
		.attr("href", "#")
		.attr("data-toggle", "popover")
		.attr("data-trigger", "hover")
		.attr("data-content", "Address is " + data["trxList"][i][3])
		.attr("data-placement", "top")
		.text(data["trxList"][i][2]);
	    td.append(a);
	    tr.append(td);
	    td = $("<td>");
	    td.text(data["trxList"][i][5]);
	    tr.append(td);
	    tBody.append(tr);
	}
	$('[data-toggle="popover"]').each(function(i, obj) {
	    var popover_target = $(this).data("popover-target");
	    $(this).popover({
		html: false,
		trigger: "click",
		placement: "top",
		container: "body",
		content: function(obj) {
		    return $(popover_target).html();
		}
	    });
	});
    });
}

$(document).ready(function(){
    getAccountData();
});

function createQR(){
    sendData = {};
    sendData["amount"]   = $("#amount").val();
    sendData["password"] = $("#password").val();
    $.ajax({
	type:"POST",
	url:"/api/createQR",
	data:JSON.stringify(sendData),
	contentType: "application/json; charset=utf-8",
	dataType: "json",
	success : function(data) {
	    if(data["status"] == "ok"){
		$("#qr").html( data["dom"] );
	    }else{
		$("#qr").html( data["error"]);
	    }
	}
    });
}

function readQR(){
    fileData = {};
    fileData = $("#qrImg").prop("files")[0];
    formData = new FormData();
    formData.append('file', fileData);
    $.ajax({
	type:"POST",
	url:"/api/readQR",
	data:formData,
	contentType: false,
	processData: false,
	success : function(data) {
	    if(data["status"] == "ok"){
		Snackbar.show({text: data["dom"]});
	    }else{
		Snackbar.show({text: data["error"]});
	    }
	}
    });
}
