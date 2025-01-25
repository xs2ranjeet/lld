#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>

enum VehicleType { CAR, TRUCK, MOTORCYCLE, VAN, ELECTRIC };
enum SpotType { COMPACT, LARGE, HANDICAPPED, MOTORCYCLE_SPOT, ELECTRIC_SPOT };
enum AccountStatus { ACTIVE, BLOCKED, BANNED, COMPROMISED, ARCHIVED, UNKNOWN };
enum TicketStatus { ACTIVE, PAID, LOST};

class Address {
public:
	std::string streetAddress;
	std::string city;
	std::string state;
	std::string zipCode;
	std::string country;
};

class Person {
public:
	std::string name;
	Address address;
	std::string email;
	std::string phone;	
};

class Account {
protected:
	std::string id;
	std::string password;
	AccountStatus status;
	Person person;

public:
	virtual bool resetPassword() = 0;
};

class Admin : public Account {
public:
	bool addParlingFloor(ParkingLot *lot,  ParkingFloor *floor) {
		// Implement add parking floor logic
		return lot->addFloor(*floor);
	}

	bool addParkingSpot(ParkingFloor *floor, ParkingSpot *spot) {
		// Implement add parking spot logic
		return floor->addSpot(*spot);
	}

	bool addParkingDisplayBoard(ParkingFloor *floor, ParkingDisplayBoard *displayBoard) {
		// Implement add parking display board logic
		return floor->addDisplayBoard(*displayBoard);
	}

	bool addCustomerInfoPanel(ParkingFloor *floor, CustomerInfoPanel *infoPanel) {
		// Implement add customer info panel logic
		return floor->addInfoPanel(*infoPanel);
	}

	bool addEntrancePanel(ParkingLot *lot, EntrancePanel *panel) {
		// Implement add entrance panel logic
		return lot->addEntrancePanel(*panel);
	}

	bool addExitPanel(ParkingLot *lot, ExitPanel *panel) {
		// Implement add exit panel logic
		return lot->addExitPanel(*panel);
	}

	bool blockUser(Account account) {
		// Implement block user logic
		account.status = BLOCKED;
		return true;
	}

	bool unblockUser(Account account) {
		// Implement unblock user logic
		account.status = ACTIVE;
		return true;
	}

	bool resetPassword() {
		// Implement password reset logic
		return true;
	}
};

class ParkingAttendant : public Account {
public:
	bool processTicket(ParkingLot *lot, ParkingTicket *ticket) {
		// Implement process ticket logic
		return true;
	}

	bool scanTicket(ParkingLot *lot, int ticketId) {
		// Implement scan ticket logic
		return true;
	}

	bool processPayment(ParkingLot *lot, ParkingTicket *ticket) {
		// Implement process payment logic
		return true;
	}

	bool resetPassword() {
		// Implement password reset logic
		return true;
	}
};

class Vehicle {
	public:
		std::string licensePlate;
		VehicleType type;
		ParkingTicket *ticket;
		Vehicle(std::string plate, VehicleType type) : licensePlate(plate), type(type) {}

		bool assignTicket(ParkingTicket *ticket) {
			// Implement assign ticket logic
			this->ticket = ticket;
			return true;
		}
	};

class ParkingSpot {
public:
    int id;
    SpotType type;
    bool isOccupied;
    bool hasElectricPanel;
	Vehicle *vehicle;

    ParkingSpot(int id, SpotType type, bool hasElectricPanel = false) 
        : id(id), type(type), isOccupied(false), hasElectricPanel(hasElectricPanel) {}

    void assignVehicle(Vehicle *vehicle) { 
		this->vehicle = vehicle;
		isOccupied = true; 
	}

	void removeVehicle() {
		isOccupied = false;
		delete this->vehicle;
		this->vehicle = nullptr;
	}
    void freeSpot() { isOccupied = false; }
};



class ParkingTicket {
public:
    int id;
    time_t entryTime;
    time_t exitTime;
    double amountPaid;
    bool isPaid;
    Vehicle* vehicle;

    ParkingTicket(int id, Vehicle* vehicle) 
        : id(id), vehicle(vehicle), amountPaid(0), isPaid(false) {
        entryTime = std::time(nullptr);
    }

    void calculateFee() {
        time_t now = std::time(nullptr);
        int hours = (now - entryTime) / 3600;
        if (hours <= 1) amountPaid = 4;
        else if (hours <= 3) amountPaid = 4 + (hours - 1) * 3.5;
        else amountPaid = 11 + (hours - 3) * 2.5;
    }

    void payTicket(double amount) {
        amountPaid = amount;
        isPaid = true;
    }
};

class ParkingFloor {
public:
    int floorNumber;
    std::vector<ParkingSpot> spots;

    ParkingFloor(int floorNumber) : floorNumber(floorNumber) {}

    void addSpot(ParkingSpot spot) {
        spots.push_back(spot);
    }

    ParkingSpot* getAvailableSpot(SpotType type) {
        for (auto& spot : spots) {
            if (!spot.isOccupied && spot.type == type) {
                return &spot;
            }
        }
        return nullptr;
    }
};

class ParkingLot {
private:
    std::string name;
    std::string address;
    int maxCapacity;
    int currentVehicleCount;
    std::vector<ParkingFloor> floors;
    std::unordered_map<int, ParkingTicket*> activeTickets;

public:
    ParkingLot(std::string name, std::string address, int maxCapacity) 
        : name(name), address(address), maxCapacity(maxCapacity), currentVehicleCount(0) {}

    void addFloor(ParkingFloor floor) {
        floors.push_back(floor);
    }

    bool isFull() {
        return currentVehicleCount >= maxCapacity;
    }

    ParkingTicket* parkVehicle(Vehicle* vehicle, SpotType spotType) {
        if (isFull()) {
            std::cout << "Parking Lot is Full\n";
            return nullptr;
        }

        for (auto& floor : floors) {
            ParkingSpot* spot = floor.getAvailableSpot(spotType);
            if (spot) {
                spot->assignVehicle();
                currentVehicleCount++;
                ParkingTicket* ticket = new ParkingTicket(activeTickets.size() + 1, vehicle);
                activeTickets[ticket->id] = ticket;
                return ticket;
            }
        }

        std::cout << "No available spot for this vehicle type.\n";
        return nullptr;
    }

    void exitVehicle(int ticketId) {
        if (activeTickets.find(ticketId) == activeTickets.end()) {
            std::cout << "Invalid Ticket\n";
            return;
        }

        ParkingTicket* ticket = activeTickets[ticketId];
        ticket->calculateFee();
        if (!ticket->isPaid) {
            std::cout << "Please pay the fee of $" << ticket->amountPaid << "\n";
            return;
        }

        for (auto& floor : floors) {
            for (auto& spot : floor.spots) {
                if (spot.isOccupied && ticket->vehicle->licensePlate == ticket->vehicle->licensePlate) {
                    spot.freeSpot();
                    break;
                }
            }
        }

        activeTickets.erase(ticketId);
        delete ticket;
        currentVehicleCount--;
        std::cout << "Vehicle exited successfully.\n";
    }
};

int main() {
    ParkingLot lot("City Parking", "123 Main St", 100);

    ParkingFloor floor1(1);
    floor1.addSpot(ParkingSpot(1, COMPACT));
    floor1.addSpot(ParkingSpot(2, ELECTRIC_SPOT, true));
    lot.addFloor(floor1);

    Vehicle car("ABC123", CAR);
    ParkingTicket* ticket = lot.parkVehicle(&car, COMPACT);

    if (ticket) {
        std::cout << "Vehicle parked with ticket ID: " << ticket->id << "\n";
    }

    lot.exitVehicle(ticket->id);

    return 0;
}
