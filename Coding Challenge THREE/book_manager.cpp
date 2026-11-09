// Challenge 3: Book Manager
#include <algorithm>
#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Book {
private:
    std::string title, author, id;
    int pages;
    bool borrowed;

public:
    Book(const std::string& bookTitle, const std::string& bookAuthor,
         int pageCount, const std::string& bookId, bool isBorrowed)
        : title(bookTitle), author(bookAuthor), id(bookId),
          pages(pageCount), borrowed(isBorrowed) {}

    const std::string& getId() const { return id; }

    void display() const {
        std::cout << "ID: " << id << " | " << title << '\n'
                  << "Author: " << author << " | Pages: " << pages << '\n'
                  << "Status: " << (borrowed ? "Borrowed" : "Available")
                  << "\n\n";
    }

    bool borrowBook() {
        if (borrowed) return false;
        borrowed = true;
        return true;
    }

    bool returnBook() {
        if (!borrowed) return false;
        borrowed = false;
        return true;
    }
};

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

Book* findBook(std::vector<Book>& books, const std::string& id) {
    for (Book& book : books) {
        if (book.getId() == id) return &book;
    }
    return nullptr;
}

bool loadBooks(const std::string& filename, std::vector<Book>& books) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open " << filename
                  << ". Run from the data file's folder or pass its path.\n";
        return false;
    }
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        if (trim(line).empty()) continue;
        std::array<std::string, 5> fields;
        std::istringstream row(line);
        bool valid = std::count(line.begin(), line.end(), ',') == 4;
        for (std::string& field : fields) {
            if (!std::getline(row, field, ',')) valid = false;
            field = trim(field);
            if (field.empty()) valid = false;
        }
        int pages = 0;
        try {
            std::size_t used = 0;
            pages = std::stoi(fields[2], &used);
            if (used != fields[2].size() || pages <= 0) valid = false;
        } catch (const std::exception&) {
            valid = false;
        }
        if (fields[4] != "true" && fields[4] != "false") valid = false;
        if (findBook(books, fields[3]) != nullptr) valid = false;
        if (!valid) {
            std::cerr << "Skipping invalid or duplicate record on line "
                      << lineNumber << ".\n";
            continue;
        }
        // IDs stay as strings so leading zeros such as 001 are preserved.
        books.emplace_back(fields[0], fields[1], pages, fields[3],
                           fields[4] == "true");
    }
    if (file.bad()) {
        std::cerr << "Could not finish reading " << filename << ".\n";
        return false;
    }
    if (books.empty()) {
        std::cerr << "No valid books were found in " << filename << ".\n";
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: book_manager [path/to/bookData.txt]\n";
        return 1;
    }
    std::vector<Book> books;
    if (!loadBooks(argc == 2 ? argv[1] : "bookData.txt", books)) return 1;
    std::cout << "Book Manager - " << books.size() << " books loaded.\n"
              << "Borrowing and returning changes last for this session only.\n";
    while (true) {
        std::cout << "\n1. View all books\n2. View one book\n"
                  << "3. Borrow a book\n4. Return a book\n0. Quit\nChoice: ";
        std::string choice;
        if (!std::getline(std::cin, choice)) break;
        choice = trim(choice);
        if (choice == "0") break;
        if (choice == "1") {
            for (const Book& book : books) book.display();
            continue;
        }
        if (choice != "2" && choice != "3" && choice != "4") {
            std::cout << "Please enter a menu number from 0 to 4.\n";
            continue;
        }
        std::cout << "Enter the book ID exactly as shown (for example 001): ";
        std::string id;
        if (!std::getline(std::cin, id)) break;
        Book* book = findBook(books, trim(id));
        if (book == nullptr) {
            std::cout << "No book has that ID.\n";
        } else if (choice == "2") {
            book->display();
        } else if (choice == "3") {
            std::cout << (book->borrowBook() ? "Book borrowed successfully.\n"
                                           : "That book is already borrowed.\n");
        } else {
            std::cout << (book->returnBook() ? "Book returned successfully.\n"
                                           : "That book is already available.\n");
        }
    }
    std::cout << "\nGoodbye.\n";
    return 0;
}
