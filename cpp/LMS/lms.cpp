#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <algorithm>

using namespace std;

// Enums for various statuses and formats
enum class BookFormat { HARDCOVER, PAPERBACK, AUDIO_BOOK, EBOOK, NEWSPAPER, MAGAZINE, JOURNAL };
enum class BookStatus { AVAILABLE, RESERVED, LOANED, LOST };
enum class ReservationStatus { WAITING, PENDING, CANCELED, NONE };
enum class AccountStatus { ACTIVE, CLOSED, CANCELED, BLACKLISTED, NONE };

// Address class to store address details
class Address {
public:
    string streetAddress, city, state, zipCode, country;
};

// Person class to store personal details
class Person {
public:
    string name, email, phone;
    Address address;
};

// Constants class to store library constants
class Constants {
public:
    static const int MAX_BOOKS_ISSUED_TO_A_USER = 5;
    static const int MAX_LENDING_DAYS = 10;
};

// Abstract Account class
class Account {
protected:
    string id, password;
    AccountStatus status;
    Person person;

public:
    virtual bool resetPassword() = 0;
};

// Librarian class inheriting from Account
class Librarian : public Account {
public:
    bool addBookItem(class BookItem& bookItem);
    bool blockMember(class Member& member);
    bool unBlockMember(class Member& member);
};

// Member class inheriting from Account
class Member : public Account {
private:
    time_t dateOfMembership;
    int totalBooksCheckedout = 0;

    void incrementTotalBooksCheckedout() { totalBooksCheckedout++; }
    void decrementTotalBooksCheckedout() { totalBooksCheckedout--; }
    void checkForFine(const string& bookItemBarcode);

public:
    int getTotalBooksCheckedout() const { return totalBooksCheckedout; }
    bool reserveBookItem(class BookItem& bookItem);
    bool checkoutBookItem(class BookItem& bookItem);
    void returnBookItem(class BookItem& bookItem);
    bool renewBookItem(class BookItem& bookItem);
};

// BookReservation class to handle book reservations
class BookReservation {
public:
    time_t creationDate;
    ReservationStatus status;
    string bookItemBarcode, memberId;

    static BookReservation* fetchReservationDetails(const string& barcode);
    void updateStatus(ReservationStatus newStatus) { status = newStatus; }
    void sendBookAvailableNotification() { /* Notify member */ }
};

// BookLending class to handle book lending
class BookLending {
public:
    time_t creationDate, dueDate, returnDate;
    string bookItemBarcode, memberId;

    static void lendBook(const string& barcode, const string& memberId);
    static BookLending* fetchLendingDetails(const string& barcode);
};

// Fine class to handle fines
class Fine {
public:
    time_t creationDate;
    double amount;
    string memberId;

    static void collectFine(const string& memberId, long days) { /* Collect fine */ }
};

// Abstract Book class
class Book {
protected:
    string ISBN, title, subject, publisher, language;
    int numberOfPages;
    vector<string> authors;
};

// BookItem class inheriting from Book
class BookItem : public Book {
private:
    string barcode;
    bool isReferenceOnly;
    time_t borrowed, dueDate;
    double price;
    BookFormat format;
    BookStatus status;
    time_t dateOfPurchase, publicationDate;
    class Rack* placedAt;

public:
    bool checkout(const string& memberId);
    void updateBookItemStatus(BookStatus newStatus) { status = newStatus; }
    bool getIsReferenceOnly() const { return isReferenceOnly; }
    string getBarCode() const { return barcode; }
    void updateDueDate(time_t newDueDate) { dueDate = newDueDate; }
};

// Rack class to store rack details
class Rack {
public:
    int number;
    string locationIdentifier;
};

// Search interface
class Search {
public:
    virtual vector<Book*> searchByTitle(const string& title) = 0;
    virtual vector<Book*> searchByAuthor(const string& author) = 0;
    virtual vector<Book*> searchBySubject(const string& subject) = 0;
    virtual vector<Book*> searchByPubDate(time_t publishDate) = 0;
};

// Catalog class implementing Search interface
class Catalog : public Search {
private:
    unordered_map<string, vector<Book*>> bookTitles;
    unordered_map<string, vector<Book*>> bookAuthors;
    unordered_map<string, vector<Book*>> bookSubjects;
    unordered_map<time_t, vector<Book*>> bookPublicationDates;

public:
    vector<Book*> searchByTitle(const string& query) override {
        return bookTitles[query];
    }

    vector<Book*> searchByAuthor(const string& query) override {
        return bookAuthors[query];
    }

    vector<Book*> searchBySubject(const string& query) override {
        return bookSubjects[query];
    }

    vector<Book*> searchByPubDate(time_t publishDate) override {
        return bookPublicationDates[publishDate];
    }
};

// Implementation of Member class methods
bool Member::checkoutBookItem(BookItem& bookItem) {
    if (getTotalBooksCheckedout() >= Constants::MAX_BOOKS_ISSUED_TO_A_USER) {
        cerr << "The user has already checked-out maximum number of books" << endl;
        return false;
    }
    BookReservation* bookReservation = BookReservation::fetchReservationDetails(bookItem.getBarCode());
    if (bookReservation && bookReservation->memberId != id) {
        cerr << "This book is reserved by another member" << endl;
        return false;
    } else if (bookReservation) {
        bookReservation->updateStatus(ReservationStatus::COMPLETED);
    }

    if (!bookItem.checkout(id)) {
        return false;
    }

    incrementTotalBooksCheckedout();
    return true;
}

void Member::checkForFine(const string& bookItemBarcode) {
    BookLending* bookLending = BookLending::fetchLendingDetails(bookItemBarcode);
    time_t dueDate = bookLending->dueDate;
    time_t today = time(0);
    if (difftime(today, dueDate) > 0) {
        long diffDays = difftime(today, dueDate) / (24 * 60 * 60);
        Fine::collectFine(id, diffDays);
    }
}

void Member::returnBookItem(BookItem& bookItem) {
    checkForFine(bookItem.getBarCode());
    BookReservation* bookReservation = BookReservation::fetchReservationDetails(bookItem.getBarCode());
    if (bookReservation) {
        bookItem.updateBookItemStatus(BookStatus::RESERVED);
        bookReservation->sendBookAvailableNotification();
    }
    bookItem.updateBookItemStatus(BookStatus::AVAILABLE);
}

bool Member::renewBookItem(BookItem& bookItem) {
    checkForFine(bookItem.getBarCode());
    BookReservation* bookReservation = BookReservation::fetchReservationDetails(bookItem.getBarCode());
    if (bookReservation && bookReservation->memberId != id) {
        cerr << "This book is reserved by another member" << endl;
        decrementTotalBooksCheckedout();
        bookItem.updateBookItemStatus(BookStatus::RESERVED);
        bookReservation->sendBookAvailableNotification();
        return false;
    } else if (bookReservation) {
        bookReservation->updateStatus(ReservationStatus::COMPLETED);
    }
    BookLending::lendBook(bookItem.getBarCode(), id);
    bookItem.updateDueDate(time(0) + Constants::MAX_LENDING_DAYS * 24 * 60 * 60);
    return true;
}

// Implementation of BookItem class methods
bool BookItem::checkout(const string& memberId) {
    if (getIsReferenceOnly()) {
        cerr << "This book is Reference only and can't be issued" << endl;
        return false;
    }
    BookLending::lendBook(getBarCode(), memberId);
    updateBookItemStatus(BookStatus::LOANED);
    return true;
}