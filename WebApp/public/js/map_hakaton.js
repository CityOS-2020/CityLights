var map;
var geocoder;
var infowindow = new google.maps.InfoWindow();
var marker;
var selectedMarkerID;

function initialize() {
	var cityLatitude = 43.855610;
	var cityLongitude = 18.395952;
	var mapZoom = 15;
	
	// Geocoder
	geocoder = new google.maps.Geocoder();
	
	// Create map - define center, zoom etc.
    var mapCanvas = document.getElementById('map-canvas');
    var mapOptions = {
        center: new google.maps.LatLng(cityLatitude, cityLongitude),
        zoom: mapZoom,
		scrollwheel: true,
		navigationControl: false,
		mapTypeControl: false,
		scaleControl: false,
		draggable: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP
    }
    map = new google.maps.Map(mapCanvas, mapOptions);
	
	
};

var dustValueLow = 0.15;
var dustValueHigh = 0.3;

function returnMarkerColor(dustLevel){
    if(dustLevel <= dustValueLow)
        return 'http://maps.google.com/mapfiles/ms/icons/green-dot.png';
    else if(dustLevel > dustValueLow && dustLevel <= dustValueHigh)
        return 'http://maps.google.com/mapfiles/ms/icons/yellow-dot.png';
    else
        return 'http://maps.google.com/mapfiles/ms/icons/red-dot.png';
}

function addHardCodedPin(loc, addressLoc){
	var latlng = new google.maps.LatLng(parseFloat(loc.latitude), parseFloat(loc.longitude));
	marker = new google.maps.Marker({
            position: latlng,
            map: map
        });
         marker.setIcon(returnMarkerColor(loc.Pollution));
		// get address
		var address = addressLoc;
		google.maps.event.addListener(marker, "click", function() {
			// Show marker details

            selectedMarkerID = loc.id;
            var contentString = getMarkerContentString(loc, address);
			if (infowindow) {
				infowindow.close();
			}
			infowindow = new google.maps.InfoWindow({
				content: contentString
			});
			infowindow.open(map, this);
		});   
	
}

function geocodeLatLng(loc) {
  var latlng = new google.maps.LatLng(parseFloat(loc.latitude), parseFloat(loc.longitude));
  geocoder.geocode({'latLng': latlng}, function(results, status) {
    if (status == google.maps.GeocoderStatus.OK) {
      if (results[1]) {
        // map.setZoom(11);
        marker = new google.maps.Marker({
            position: latlng,
            map: map
        });
         marker.setIcon(returnMarkerColor(loc.Pollution));
		// get address
		var address = results[1].formatted_address;
		google.maps.event.addListener(marker, "click", function() {
			// Show marker details

            selectedMarkerID = loc.id;
            var contentString = getMarkerContentString(loc, address);
			if (infowindow) {
				infowindow.close();
			}
			infowindow = new google.maps.InfoWindow({
				content: contentString
			});
			infowindow.open(map, this);
		});      
      } 
	  else {
        console.log('No results found');
      }
    } 
	else {
      console.log('Geocoder failed due to: ' + status);
    }
  });
};

function removeState(address){
    var stringSplit = address.split(',').reverse();
    stringSplit.splice(stringSplit.lenght - 1, 1);
    stringSplit.reverse();
    var newString = "";
    for(var i = 0; i < stringSplit.length; i++)
        newString = newString + stringSplit[i] + ', ';
    newString = newString.substring(0, newString.length - 2);
    return newString;
}

function getMarkerContentString(loc, address)
{
	var temperature = loc.Temperature;
	var humidity = loc.Humidity;
	var pollution = loc.Pollution;
	var imgHtml;
	if(loc.isPrivateProperty)
		imgHtml = '<img src="../images/private_icon.svg" alt="Private property" class="station-type-image">';
	else 
		imgHtml = '<img src="../images/public_icon.svg" alt="Public property" class="station-type-image">';
	var content = '<div class="popup-outer">' + imgHtml + '<div class="popup-content"> <span class="page-role-text-subs">' + removeState(address) + '</span>' +
		'<table class="data-table">' +
		'<tr><td class="page-role-text data-caption">Temperature</td>' +
		'<td class="page-role-text data">' + '<img src="../images/temperature_icon.svg" alt="Temperature icon" class="data-icon">' + temperature + ' °C' + '</td></tr>' +
		
		'<tr><td class="page-role-text data-caption">Humidity</td>' +
		'<td class="page-role-text data">' + '<img src="../images/humidity_icon.svg" alt="Humidity icon" class="data-icon">' + humidity + ' %' + '</td></tr>' +
		
		'<tr><td class="page-role-text data-caption">Pollution</td>' +
		'<td class="page-role-text data">' + '<img src="../images/dust_icon.svg" alt="Dust icon" class="data-icon">' + pollution + ' mg/m3' + '</td></tr>' +
		'</table></div></div>';
	return content;
};

