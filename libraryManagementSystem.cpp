#include <iostream>
// standard template libraries
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// helper function to convert a string to lowercase
string toLower(const string &str) {
    string lowerStr = str;
    // transform each character of the input string to lowercase
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// class 'Book'
class Book {
private:
    // private variables to store the book's title, author, and genre
    string title;
    string author;
    string genre;
public:
    // constructor that initializes the book's title, author, and genre
    Book(string t, string a, string g): title(t), author(a), genre(g) {}

    // constant member function to display book details
    void displayBook() const {
        cout << "Title: " << title << endl << "Author: " << author << endl << "Genre: " << genre << endl;
    }

    // getter function to return the title of the book
    inline string getTitle() const {
        return title;
    }

    // getter function to return the genre of the book
    inline string getGenre() const {
        return genre;
    }
};

// class 'Member'
class Member {
private:
    // private variables to store the member's name and id
    string name;
    int id;
public:
    // constructor that initializes the member's name and id
    Member(string n, int i) : name(n), id(i) {}

    // constant member function to display member details
    void displayMember() const {
        cout << "Name: " << name << ", ID: " << id;
    }

    // getter function to return the member's name
    inline string getName() const {
        return name;
    }

    // getter function to return the member's id
    inline int getID() const {
        return id;
    }
};

// class 'Library'
class Library {
private:
    // vector to store a collection of books
    vector<Book> books;
    // vector to store a collection of members
    vector<Member> members;
    
    // recursive function to find a book by title (case-insensitive)
    // starts searching from the index provided
    int findBookByTitle(const string &title, int index = 0) const {
        // base case: if index reaches the end of the books vector, return -1 (book not found)
        if (index >= books.size()) {
            return -1;
        }
        // if the current book's title matches the search title (case-insensitive), return the index
        if (toLower(books[index].getTitle()) == toLower(title)) {
            return index;
        }
        // recursively call the function with the next index
        return findBookByTitle(title, index + 1);
    }

    // function to check if a member's id is unique
    // iterates through the members to ensure no member has the same id
    bool isIDUnique(int id) const {
        for (const Member& member : members) {
            if (member.getID() == id) {
                return false;  // if id is not unique, return false
            }
        }
        return true;  // if id is unique, return true
    }

    // friends allow these functions to access private members of the 'library' class
    friend void issueBookFromSearch(Library &library, const Member &member);
    friend Member addMemberFromUser(Library &library);

public:
    // function to add a new book to the library's collection
    void addBook(const Book &book) {
        books.push_back(book);
    }

    // function to add a new member to the library's collection
    void addMember(const Member &member) {
        members.push_back(member);
    }

    // function to issue a book to a member by providing the book object
    void issueBook(Book book, Member member) {
        cout << "\n" << member.getName() << " issued the book titled: " << book.getTitle() << endl;
    }

    // function to issue a book to a member by providing the genre of the book
    void issueBook(string genre, Member member) {
        cout << member.getName() << " issued a book with genre: " << genre << endl;
    }

    // function to display all books in the library's collection
    void displayBooks() const {
        cout << "\nList of Books:\n";
        // loop through each book in the collection and display its details
        for (const auto &book : books) {
            book.displayBook();
            cout << endl;
        }
    }

    // function to display all members of the library
    void displayMembers() const {
        cout << "List of Members:\n";
        // loop through each member in the collection and display their details
        for (const auto &member : members) {
            member.displayMember();
            cout << endl;
        }
    }

    // function to search for a book by title and display its details if found
    void searchBookByTitle(const string &title) const {
        int index = findBookByTitle(title);  // search for the book by title
        if (index != -1) {
            // if the book is found, display its details
            cout << "Book found: \n";
            books[index].displayBook();
        } else {
            // if the book is not found, display a message
            cout << "Book not found with the title: " << title << endl;
        }
    }
};

// namespace 'LMSOperations' for performing library-related operations
namespace LMSOperations {
    // function to add a book to the library
    void addBook(Library &library, const Book &book) {
        library.addBook(book);
    }

    // function to add a member to the library
    void addMember(Library &library, const Member &member) {
        library.addMember(member);
    }
}

// function to add a new member to the library by taking user input
Member addMemberFromUser(Library &library) {
    string name;
    int id = 0;
    cout << "Enter your name: ";
    getline(cin, name);
    // loop until the user enters a valid unique positive integer id
    bool condition = true;
    while (condition) {
        cout << "Enter your ID (positive integer): ";
        cin >> id;
        
        if (id > 0) {
            // check if the id is unique
            if (!library.isIDUnique(id)) {
                cout << "ID " << id << " is already taken. Please enter a different ID.\n";
            } else {
                condition = false;  // valid and unique id, exit the loop
            }
        } else {
            cout << "Invalid input. Please enter a positive integer.\n";
        }
        cin.ignore();  // consume the remaining newline character
    }
    // create a new member and add them to the library
    Member newMember(name, id);
    LMSOperations::addMember(library, newMember);
    return newMember;
}

// function to search for and issue a book to a member
void issueBookFromSearch(Library &library, const Member &member) {
    string bookTitle;

    // loop to prompt for a book title or terminate
    while (true) {
        cout << "\nEnter the name of the book you would like or -1 to terminate: ";
        getline(cin, bookTitle);

        // break if the user enters -1
        if (bookTitle == "-1") {
            cout << "Program Termination.\n";
            break;
        }

        // search for the book by title (case-insensitive)
        int index = library.findBookByTitle(bookTitle);
        
        if (index != -1) { // if the book is found
            cout << "Would you like to issue this book?\nIf yes, Press Y: \nIf no, press any other button: ";
            char choice;
            cin >> choice;
            cin.ignore();  // consume the remaining newline character

            if (choice == 'y' || choice == 'Y') {
                // issue the book to the member
                library.issueBook(library.books[index], member);
            } else {
                cout << "Book not issued.\n";
            }
        } else {
            // if the book is not found, display a message
            cout << "Book not found with the title: " << bookTitle << endl;
        }
    }
}

int main() {
    Library library;

    // create book and member objects
    Book book1("The Great Gatsby", "F. Scott Fitzgerald", "International Classics");
    Book book2("Anna Karenina", "L. Tolstoy", "Russian Classics");
    Book book3("To Kill a Mockingbird", "Harper Lee", "American Classics");

    Member member1("John Doe", 1);
    Member member2("Whimsy Lou", 2);
    Member member3("Nicola Tesla", 3);

    // add books and existing members to the library
    LMSOperations::addBook(library, book1);
    LMSOperations::addBook(library, book2);
    LMSOperations::addBook(library, book3);

    LMSOperations::addMember(library, member1);
    LMSOperations::addMember(library, member2);
    LMSOperations::addMember(library, member3);

    // prompt user to add themselves as a new member and store that member
    Member newUser = addMemberFromUser(library);

    // display all books and members
    library.displayBooks();
    library.displayMembers();

    // issue books by object and by genre
    library.issueBook(book1, member1);  // issuing by book object
    library.issueBook("Russian Classics", member2);  // issuing by genre (overloaded function)

    // allow the user to search for a book and issue it if found
    issueBookFromSearch(library, newUser);

    return 0;
}
