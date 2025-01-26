//
//  UserModel.swift
//  ParkingLot
//
//  Created by Ranjeet Kumar on 26/01/25.
//

import Foundation

// Your model class must conform to Identifiable protocol in order to show the users in List view.
class UserModel: Identifiable {
	public var id: Int64 = 0
	public var name: String = ""
	public var email: String = ""
	public var password: String = ""
	public var age: Int64 = 0
}
