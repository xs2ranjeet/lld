//
//  ContentView.swift
//  ParkingLot
//
//  Created by Ranjeet Kumar on 25/01/25.
//

import SwiftUI

struct ContentView: View {
	
	@State var userModels: [UserModel] = []
	@State var userSelected: Bool = false
	@State var selectedUserId: Int64 = 0
	var body: some View {
		NavigationView {
			VStack {
			// create link to add user
				NavigationLink (destination: AddUserView(), label: {
					Text("Add user")
				})
				   HStack {
					   NavigationLink (destination: EditUserView(id: self.$selectedUserId), isActive: self.$userSelected) {
						   EmptyView()
					   }
					   List(self.userModels) { (model) in
						   HStack {
							   Text(model.name)
							   Spacer()
							   Text(model.email)
							   Spacer()
							   Text("\(model.age)")
							   Spacer()
							   
							   Button(action: {
								   self.selectedUserId = model.id
								   self.userSelected = true
							   }, label: {
								   Text("Edit")
									   .foregroundColor(.blue)
							   })
							   .buttonStyle(PlainButtonStyle())
							   
							   Button(action: {
								   let db: DB_Manager = DB_Manager()
								   db.deleteUser(idValue: model.id)
								   self.userModels = db.getUsers()
							   }, label: {
								   Text("Delete")
									   .foregroundColor(.red)
							   })
							   .buttonStyle(PlainButtonStyle())
						   }
					   }
						   
				   }
			}
			.onAppear(perform: {
				self.userModels = DB_Manager().getUsers()
			})
		}
		.padding()
		.navigationBarTitle("Parking Lot")
	}
}

#Preview {
    ContentView()
}
