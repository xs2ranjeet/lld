#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Book {
private:
    string title;
    string isbn;
    string author;
    int pubdate;
    bool available = true;
public:
    Book(string isbn, string title, string author, int pubdate) 
        : isbn(isbn), title(title), author(author), pubdate(pubdate) {}

    string getTitle() const {
        return title;
    }

    string getIsbn() const {
        return isbn;
    }

    string getAuthor() const {
        return author;
    }

    int getPubdate() const {
        return pubdate;
    }
    void setAvailable(bool available) {
        this->available = available;
    }

    bool isAvailable() {
        return available;
    }
};

class Member {
private:
    string memberId;
    string name;
    string email;
    vector<Book*> borrowedBooks;
public:
    Member(string memberId, string name, string email) 
        : memberId(memberId), name(name), email(email) {}

    string getMemberId() const {
        return memberId;
    }

    string getName() const {
        return name;
    }

    string getEmail() const {
        return email;
    }
    void borrowBook(Book* book) {
        borrowedBooks.push_back(book);
        book->setAvailable(false);
    }

    void returnBook(Book* book) {
        borrowedBooks.erase(remove(borrowedBooks.begin(), borrowedBooks.end(), book), borrowedBooks.end());
        book->setAvailable(true);
    }

    vector<Book*> getBorrowedBooks() {
        return borrowedBooks;
    }
};

class LibraryManager {
private:
    unordered_map<string, Book*> books;
    unordered_map<string, Member*> members;
    static LibraryManager* instance;
    LibraryManager() {
        books = {};
        members = {};
    }
public:
    static LibraryManager* getInstance() {
        if (instance == nullptr) {
            instance = new LibraryManager();
        }
        return instance;
    }

    void addBook(Book* book) {
        books[book->getIsbn()] = book;
    }

    void registerMember(Member* member) {
        members[member->getMemberId()] = member;
    }

    void unregisterMember(const string& memberId) {
        members.erase(memberId);
    }   

    Member *getMember(const string& memberId) {
        return members[memberId];
    }

    Book *getBook(const string& isbn) {
        return books[isbn];
    }

    void borrowBook(const string& memberId, const string& isbn) {
        // Implement borrowing logic
        Member *member = getMember(memberId);
        Book *book = getBook(isbn);
        if (member == nullptr || book == nullptr) {
            cout << "Invalid member or book\n";
            return;
        }
        if (!book->isAvailable()) {
            cout << "Book is not available\n";
            return;
        }
        member->borrowBook(book);
        book->setAvailable(false);
    }

    void returnBook(const string& memberId, const string& isbn) {
        // Implement return logic
        Member *member = getMember(memberId);
        Book *book = getBook(isbn);
        if (member == nullptr || book == nullptr) {
            cout << "Invalid member or book\n";
            return;
        }
        member->returnBook(book);
        book->setAvailable(true);
    }

    vector<Book*> searchBook(const string &bookName) {
        vector<Book*> result;
        for (auto &book : books) {
            if (book.second->getTitle().find(bookName) != string::npos) {
                result.push_back(book.second);
            }
        }
        return result;
    }
};

LibraryManager* LibraryManager::instance = nullptr;
int main() {
    LibraryManager *libraryManager = LibraryManager::getInstance();

    // Add books to the catalog
    libraryManager->addBook(new Book("ISBN1", "Book1", "Auth1", 2020));
    libraryManager->addBook(new Book("ISBN2", "Book2", "Auth2", 2021));
    libraryManager->addBook(new Book("ISBN3", "Book3", "Auth3", 2022));

    // Register Member
    libraryManager->registerMember(new Member("M1", "John Doe", "jdoe@test.com"));
    libraryManager->registerMember(new Member("M2", "Ranjeet", "ranjeet@test.com"));
   
    libraryManager->borrowBook("M1", "ISBN1");
    libraryManager->borrowBook("M2", "ISBN2");

    libraryManager->returnBook("M1", "ISBN1");

    // Search Book
    vector<Book*> searchResult = libraryManager->searchBook("Book");
    for(auto &result : searchResult) {
        cout << result->getTitle() << "\n";
    }

    delete libraryManager;
    return 0;
}