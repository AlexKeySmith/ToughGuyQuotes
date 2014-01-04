var ToughGuyQuotes = function(Pebble){
	
	var settingsVersionKey = "4QPF";
	
	var getSettingsURL = "http://x.setpebble.com/api/" + settingsVersionKey + "/" + Pebble.getAccountToken();
	var setSettingsURL = "http://x.setpebble.com/" + settingsVersionKey + "/" + Pebble.getAccountToken();

	var defaultSettings = {
		'firstName' : 'Alex',
		'lastName' : 'Key',
		'showExplicit' : 0,
		'default' : true
	}
	
	var settings = window.localStorage[settingsVersionKey] ? JSON.parse(window.localStorage[settingsVersionKey]) : defaultSettings;
	
	var req = new XMLHttpRequest();
	
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			//remove the \' escape sequence as it's invalid in json.
			var removeSingleQuoteEscape = req.responseText.replace("\\'", "'")

			var response = JSON.parse(removeSingleQuoteEscape);
			var text = response.value[0].joke;
			Pebble.showSimpleNotificationOnPebble("Tough Guy", text);	
			
		} else {
			 Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't load the quotes");
		}	
	}
	req.ontimeout = function(e) {
		Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't load the quotes it timed out connecting to the internet");
	}
	
	
	req.timeout = 3000;
	
	 
	var getQuotes = function() {

		if(settings.default) {
			Pebble.showSimpleNotificationOnPebble("Tough Guy", "You can set your own name in the pebble app to customise the quotes.");
		}
		
		var count = 1;
		
		var firstName = encodeURIComponent(settings.firstName);
		var lastName = encodeURIComponent(settings.lastName);
		
		var url = "http://api.icndb.com/jokes/random/" + count + "?escape=javascript&firstName=" + firstName + "&lastName=" + lastName;
		
		req.open('GET', url, true);
	 	req.send(null);
	};
	

	var saveSettings = function(settingsJSON) {
	
		try {
			
			//n.b. settingsJSON not an array, but the key is a number
			var firstName = settingsJSON["1"] || "";
			var lastName = settingsJSON["2"] || "";
			var showExplicit = null;
			
			if(settingsJSON["3"] === 1) {
				showExplicit = true;
			} else if (settingsJSON["3"] === 0) {
				showExplicit = false;
			}
			
			if(firstName.length > 0 && lastName.length > 0 && showExplicit !== null) {
				
				settings = {
					"firstName" : firstName,
					"lastName" : lastName,
					"showExplicit" : showExplicit,
					"default" : false
				};

				var settingsString = JSON.stringify(settings);
				window.localStorage[settingsVersionKey] = settingsString;
				
			} else {
				Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't save your name etc in the settings, please ensure you have your first name, last name and if explicit quotes are on or not.");
			}
			
		
			
		} catch(e) {
			console.log(e);
			Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't save your name etc in the settings.");
			settings = defaultSettings;
	    }
		
	};

	Pebble.addEventListener("showConfiguration", function(e) {
		Pebble.openURL(setSettingsURL);
	});
	
	
	Pebble.addEventListener("webviewclosed", function(e) {
	
		if ((typeof(e.response) == 'string') && (e.response.length > 0)) {
			console.log(e.response);
			var settingsJSON = JSON.parse(e.response);
			saveSettings(settingsJSON);
			
		}  else {		
			Pebble.showSimpleNotificationOnPebble("Tough Guy", "Sorry couldn't find your settings on the settings page.");
		}

	});
	
	Pebble.addEventListener("appmessage", function(e) {
		
		getQuotes();
	});
	
	Pebble.addEventListener("ready", function(e) {
		
		
		if( window.localStorage[settingsVersionKey].default == true ) {
			Pebble.showSimpleNotificationOnPebble("Tough Guy", "You can set your own name in the Pebble App settings");
		}
		
	});
	
	
}(Pebble);

