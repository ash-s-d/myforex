// Set callback for the app ready event
Pebble.addEventListener(
	"ready",
	function(e) {
		fetchForexData();

		//Pebble.showSimpleNotificationOnPebble("TEST", "Test");
	}
);

// Set callback for appmessage events
Pebble.addEventListener(
	"appmessage",
	function(e)	{
		fetchForexData();
	}
);

// Fetch forex data from http://domah.com/forex
function fetchForexData() {
	var req = new XMLHttpRequest();

	// build the GET request
	req.open("GET", "http://domah.com/forex/", true);

	req.onload =	function(e) {
									// 200 - HTTP OK
									if(req.readyState == 4 && req.status == 200) {
										var response = JSON.parse(req.responseText);

										for(var currency in response.rates){
											var data = [];

											data = {
														"date": String(response.date),
														"currency": String(currency),
														"buy_rate": String(response.rates[currency].BUY_NOTES),
														"sell_rate": String(response.rates[currency].SELL_NOTES)
											};

											Pebble.sendAppMessage(data);
										}
									}else{
										console.log("Error!");
									}
					};

	req.onerror = function(e) {
		console.log("Error!");
	};

	req.send(null);
}