//
//  AppDelegate.swift
//  sclApp1.0
//
//  Created by System Administrator on 14/03/15.
//  Copyright (c) 2015 System Administrator. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {
    
    var window: UIWindow?
    // 1
    let googleMapsApiKey = "AIzaSyBTzyZYpAps06KvQCuM6rYq3h8GM89RHog"
    
    func application(application: UIApplication!, didFinishLaunchingWithOptions launchOptions: NSDictionary!) -> Bool {
        // 2
        GMSServices.provideAPIKey(googleMapsApiKey)
        return true
    }
}