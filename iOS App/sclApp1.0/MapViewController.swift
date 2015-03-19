//
//  MapViewController.swift
//  sclApp1.0
//
//  Created by System Administrator on 14/03/15.
//  Copyright (c) 2015 System Administrator. All rights reserved.
//

import UIKit

class MapViewController: UIViewController, CLLocationManagerDelegate, GMSMapViewDelegate {
    
    @IBOutlet weak var addressLabel: UILabel!
    @IBOutlet weak var mapView: GMSMapView!
    @IBOutlet weak var mapCenterPinImage: UIImageView!
    @IBOutlet weak var pinImageVerticalConstraint: NSLayoutConstraint!
    //var searchedTypes = ["bakery", "bar", "cafe", "grocery_or_supermarket", "restaurant"]
    
    let locationManager = CLLocationManager()
    override func viewDidLoad() {
        super.viewDidLoad()
        let urlAsString = "http://192.168.224.104:3000/load"
        let url = NSURL(string: urlAsString)!
        
        let urlSession = NSURLSession.sharedSession()
        
        locationManager.delegate = self
        locationManager.requestWhenInUseAuthorization()
        
        var marker = GMSMarker()
        
        marker.appearAnimation = kGMSMarkerAnimationPop
        marker.icon = UIImage(named: "flag_icon")
        
        var mutableURLRequest = NSMutableURLRequest(URL: url)
        mutableURLRequest.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        let jsonQuery = urlSession.dataTaskWithRequest(mutableURLRequest, completionHandler: { data, response, error -> Void in
            if (error != nil) {
                println(error.localizedDescription)
            }
            var err: NSError?
            
            var json = JSON(data: data, options: NSJSONReadingOptions.AllowFragments, error: nil)
            let lastIndex = json.count
            
            var id : Int = json[0]["id"].intValue
            var latitude : Double = json[0]["latitude"].doubleValue
            var longitude : Double = json[0]["longitude"].doubleValue
            var temperature : Double = json[0]["Temperature"].doubleValue
            var humidity : Double = json[0]["Humidity"].doubleValue
            var pollution : Double = json[0]["Pollution"].doubleValue
            
            
            dispatch_async(dispatch_get_main_queue(), {
                marker.position = CLLocationCoordinate2DMake(latitude, longitude)
                marker.snippet = "Temperature: \(temperature)  \nHumidity: \(humidity) \nPollution: \(pollution)"
            switch(pollution)
                {
                case 0...0.15:
                    marker.icon = GMSMarker.markerImageWithColor(UIColor.greenColor())
                case 0.15...0.3:
                    marker.icon = GMSMarker.markerImageWithColor(UIColor.orangeColor())
                default:
                    marker.icon = GMSMarker.markerImageWithColor(UIColor.redColor())
                
                }
               
            
            })
        })
        // 5
        jsonQuery.resume()
       
        var MarinDvor = GMSMarker()
        MarinDvor.appearAnimation = kGMSMarkerAnimationPop
        MarinDvor.icon = UIImage(named: "flag_icon")
        MarinDvor.snippet = "Temperature: 25 \nHumidity: 20 \nPollution: 0.01"
        MarinDvor.position = CLLocationCoordinate2DMake(43.85575, 18.40699)
        MarinDvor.icon = GMSMarker.markerImageWithColor(UIColor.greenColor())
        
        
        var Pofalici = GMSMarker()
        Pofalici.appearAnimation = kGMSMarkerAnimationPop
        Pofalici.icon = UIImage(named: "flag_icon")
        Pofalici.snippet = "Temperature: 25 \nHumidity: 20 \nPollution: 0.21"
        Pofalici.position = CLLocationCoordinate2DMake(43.85395, 18.39184)
        Pofalici.icon = GMSMarker.markerImageWithColor(UIColor.orangeColor())
        
        
        var Grbavica = GMSMarker()
        Grbavica.appearAnimation = kGMSMarkerAnimationPop
        Grbavica.icon = UIImage(named: "flag_icon")
        Grbavica.snippet = "Temperature: 25 \nHumidity: 20 \nPollution: 0.11"
        Grbavica.position = CLLocationCoordinate2DMake(43.85143, 18.39832)
        Grbavica.icon = GMSMarker.markerImageWithColor(UIColor.greenColor())
        
        
        var MarkaMarulica = GMSMarker()
        MarkaMarulica.appearAnimation = kGMSMarkerAnimationPop
        MarkaMarulica.icon = UIImage(named: "flag_icon")
        MarkaMarulica.snippet = "Temperature: 25 \nHumidity: 20 \nPollution: 0.13"
        MarkaMarulica.position = CLLocationCoordinate2DMake(43.85592, 18.38274)
        MarkaMarulica.icon = GMSMarker.markerImageWithColor(UIColor.greenColor())
        
        
        Pofalici.map = mapView
        MarkaMarulica.map = mapView
        Grbavica.map = mapView
        MarinDvor.map = mapView
        
        
        marker.map = mapView
        
        mapView.delegate = self
        
        }
       func locationManager(manager: CLLocationManager!, didChangeAuthorizationStatus status: CLAuthorizationStatus) {
        // 2
        if status == .AuthorizedWhenInUse {
            
            // 3
            locationManager.startUpdatingLocation()
            
            //4
            mapView.myLocationEnabled = true
            mapView.settings.myLocationButton = true
        }
    }
    
    // 5
    func locationManager(manager: CLLocationManager!, didUpdateLocations locations: [AnyObject]!) {
        if let location = locations.first as? CLLocation {
            
            // 6
            mapView.camera = GMSCameraPosition(target: location.coordinate, zoom: 15, bearing: 0, viewingAngle: 0)
            
            // 7
            locationManager.stopUpdatingLocation()
        }
    }
    
    func reverseGeocodeCoordinate(coordinate: CLLocationCoordinate2D) {
        
        let labelHeight = self.addressLabel.intrinsicContentSize().height
        self.mapView.padding = UIEdgeInsets(top: self.topLayoutGuide.length, left: 0, bottom: labelHeight, right: 0)
        // 1
        let geocoder = GMSGeocoder()
        
        // 2
        geocoder.reverseGeocodeCoordinate(coordinate) { response , error in
            if let address = response?.firstResult() {
                
                // 3
                let lines = address.lines as [String]
                self.addressLabel.text = join("\n", lines)
                
                // 4
                UIView.animateWithDuration(0.25) {
                    self.pinImageVerticalConstraint.constant = ((labelHeight - self.topLayoutGuide.length) * 0.5)
                    self.view.layoutIfNeeded()
                }
            }
        }
    }
    
    func mapView(mapView: GMSMapView!, idleAtCameraPosition position: GMSCameraPosition!) {
        reverseGeocodeCoordinate(position.target)
    }
    
       
    
}

