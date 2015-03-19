//
//  RangListaViewController.swift
//  sclApp1.0
//
//  Created by System Administrator on 14/03/15.
//  Copyright (c) 2015 System Administrator. All rights reserved.
//

import UIKit

class RangListaViewController: UITableViewController {
    @IBAction func rankExit(sender: UIBarButtonItem) {
    }

    var streets = [Ulica]()
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
                self.streets = [Ulica(adresa:  "Marin Dvor                                           0.01"), Ulica(adresa: "Grbavica                                              0.11 "), Ulica(adresa: "Marka Marulica                                    0.13"), Ulica(adresa:"Pofalici                                                 0.21")]
        
        
           }
        override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.streets.count
    }
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = self.tableView.dequeueReusableCellWithIdentifier("Cell", forIndexPath: indexPath) as UITableViewCell
        var street : Ulica
        street = streets[indexPath.row]
        
        
        
        
        cell.textLabel?.text = street.adresa
        return cell
    }
    
    
}
