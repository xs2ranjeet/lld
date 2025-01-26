//
//  EditUserView.swift
//  ParkingLot
//
//  Created by Ranjeet Kumar on 26/01/25.
//

import SwiftUI

struct EditUserView: View {
	
	// id receiving of user from previous view
	@Binding var id: Int64
	
	// create variables to store user input values
	@State private var name = ""
	@State private var email = ""
	@State private var password = ""
	@State private var age = ""
	
	// to go back on the home screen when the user is added
	@Environment(\.presentationMode) var mode: Binding<PresentationMode>
	
	var body: some View {
		VStack {
			// Create name field
			TextField("Enter name", text: $name)
				.textFieldStyle(RoundedBorderTextFieldStyle())
				.padding(10)
				.background(Color(.systemGray6))
				.cornerRadius(5)
				.disableAutocorrection(true)
			
			TextField("Enter email", text: $email)
				.textFieldStyle(RoundedBorderTextFieldStyle())
				.padding(10)
				.background(Color(.systemGray6))
				.cornerRadius(5)
				.disableAutocorrection(true)
				.keyboardType(.emailAddress)
				.autocapitalization(.none)
			
			TextField("Enter password", text: $password)
				.textFieldStyle(RoundedBorderTextFieldStyle())
				.padding(10)
				.background(Color(.systemGray6))
				.cornerRadius(5)
				.disableAutocorrection(true)
				.autocapitalization(.none)
				.textContentType(.password)
			
			TextField("Enter age", text: $age)
				.textFieldStyle(RoundedBorderTextFieldStyle())
				.padding(10)
				.background(Color(.systemGray6))
				.cornerRadius(5)
				.disableAutocorrection(true)
				.keyboardType(.numberPad)

			Button(action: {
				DB_Manager().updateUser(idValue: self.id,  name: name, email: email, password: password, age: Int64(age) ?? 0)
				self.mode.wrappedValue.dismiss()
			}, label: {
				Text("Edit User")
			})
			.frame(maxWidth: .infinity, alignment: .trailing)
			.padding(.top , 10)
			.padding(.bottom, 10)
		}.padding()
			.onAppear(){
				let userModal: UserModel = DB_Manager().getUser(id: self.id)
				self.name = userModal.name
				self.email = userModal.email
				self.password = userModal.password
				self.age = "\(userModal.age)"
			}
	}
}

struct EditUserView_Previews: PreviewProvider {
	 
	// when using @Binding, do this in preview provider
	@State static var id: Int64 = 0
	 
	static var previews: some View {
		EditUserView(id: $id)
	}
}
