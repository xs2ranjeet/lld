//
//  DB_Manager.swift
//  ParkingLot
//
//  Created by Ranjeet Kumar on 26/01/25.
//

import Foundation
import SQLite

class DB_Manager {
	private var db: Connection!
	private var users: Table!
	
	private let dbCreateKey = "DB_CREATE"
	
    private var id: SQLite.Expression<Int64>!
    private var name: SQLite.Expression<String>!
    private var email: SQLite.Expression<String>!
    private var password: SQLite.Expression<String>!
    private var age: SQLite.Expression<Int64>!
	
    init() {
        do {
            let path = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true).first!
            db = try Connection("\(path)/ParkingLot.sqlite3")
            users = Table("users")
            id = SQLite.Expression<Int64>("id")
            name = SQLite.Expression<String>("name")
            email = SQLite.Expression<String>("email")
            password = SQLite.Expression<String>("password")
            age = SQLite.Expression<Int64>("age")
            
            if(!UserDefaults.standard.bool(forKey: dbCreateKey)) {
                try db.run(users.create { (t) in
                    t.column(id, primaryKey: true)
                    t.column(name)    // Name of the user
                    t.column(email, unique: true)    // Email of the user
                    t.column(password)    // Password of the user
                    t.column(age)    // Age of the user
                })
                
                UserDefaults.standard.set(true, forKey: dbCreateKey)
            }
        } catch {
            print(error.localizedDescription)
        }
    }
	
	public func addUser(name: String, email: String, password: String, age: Int64) {
		do {
			try db.run(users.insert(self.name <- name, self.email <- email, self.password <- password, self.age <- age))
		} catch {
			print(error.localizedDescription)
		}
	}
	
	public func getUsers() -> [UserModel] {
		
		var userModels: [UserModel] = []
		
		users = users.order(id.desc)
		do {
			for user in try db.prepare(users) {
				let userModel: UserModel = UserModel()
				
				userModel.id = user[id]
				userModel.name = user[name]
				userModel.email = user[email]
				userModel.password = user[password]
				userModel.age = user[age]
				
				userModels.append(userModel)
			}
		} catch {
			print(error.localizedDescription)
		}
		return userModels
	}
	
	public func getUser(id: Int64) -> UserModel {
		let userModel: UserModel = UserModel()
		do {
			let user: AnySequence<Row> = try db.prepare(users.filter(self.id == id))
			try user.forEach { (row) in
				userModel.id = row[self.id]
				userModel.name = row[self.name]
				userModel.email = row[self.email]
				userModel.password = row[self.password]
				userModel.age = row[self.age]
			}
		} catch {
			print(error.localizedDescription)
		}
		
		return userModel
	}
	
	public func updateUser(idValue: Int64, name: String, email: String, password: String, age: Int64) {
		do {
			let user: Table = users.filter(id == idValue)
			
			try db.run(user.update(self.name <- name, self.email <- email, self.password <- password, self.age <- age))
		} catch {
			print(error.localizedDescription)
		}
	}
	
	public func deleteUser(idValue: Int64) {
		do {
			let user: Table = users.filter(id == idValue)
			try db.run(user.delete())
		} catch {
			print(error.localizedDescription)
		}
	}
}
