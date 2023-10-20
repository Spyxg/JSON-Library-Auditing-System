#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"
#include <ctime>
#include <limits>

using json = nlohmann::json; // Alias for the JSON library.

// Define a structure for a book.
struct Book {
    std::string name;
    std::string genre;
    std::string author; // For outside-library books, store the name of the person taking the book.
    std::string dueDate; // For outside-library books.
};

// Define a structure for the library database.
struct LibraryDatabase {
    std::vector<Book> books;
};

// Define a structure for the outside-library database.
struct OutsideLibraryDatabase {
    std::vector<Book> outsideBooks;
};

// Define a structure for the overdue books database.
struct OverdueDatabase {
    std::vector<Book> overdueBooks;
};

// Function to add a book to the library.
void AddBookToLibrary(LibraryDatabase& library, const Book& book) {
    library.books.push_back(book);
}

// Function to remove a book from the library by its name.
void RemoveBookFromLibrary(LibraryDatabase& library, const std::string& bookName) {
    auto it = library.books.begin();
    while (it != library.books.end()) {
        if (it->name == bookName) {
            it = library.books.erase(it);
        }
        else {
            ++it;
        }
    }
}

// Function to add a book to the outside library.
void AddBookToOutsideLibrary(OutsideLibraryDatabase& outsideLibrary, const Book& book) {
    outsideLibrary.outsideBooks.push_back(book);
}

// Function to move overdue books to the overdue database.
void MoveOverdueBooks(OutsideLibraryDatabase& outsideLibrary, OverdueDatabase& overdue, const std::string& currentDate) {
    auto it = outsideLibrary.outsideBooks.begin();
    while (it != outsideLibrary.outsideBooks.end()) {
        if (it->dueDate < currentDate) {
            overdue.overdueBooks.push_back(*it);
            it = outsideLibrary.outsideBooks.erase(it);
        }
        else {
            ++it;
        }
    }
}

// Function to serialize a data structure to JSON.
json SerializeToJson(const LibraryDatabase& library) {
    json jsonLibrary;
    for (const auto& book : library.books) {
        json jsonBook;
        jsonBook["name"] = book.name;
        jsonBook["genre"] = book.genre;
        jsonBook["dueDate"] = book.dueDate;
        jsonLibrary.push_back(jsonBook);
    }
    return jsonLibrary;
}

// Function to serialize a data structure to JSON.
json SerializeToJson(const OutsideLibraryDatabase& outsideLibrary) {
    json jsonLibrary;
    for (const auto& book : outsideLibrary.outsideBooks) {
        json jsonBook;
        jsonBook["name"] = book.name;
        jsonBook["genre"] = book.genre;
        jsonBook["author"] = book.author;
        jsonBook["dueDate"] = book.dueDate;
        jsonLibrary.push_back(jsonBook);
    }
    return jsonLibrary;
}

// Function to serialize a data structure to JSON.
json SerializeToJson(const OverdueDatabase& overdue) {
    json jsonOverdue;
    for (const auto& book : overdue.overdueBooks) {
        json jsonBook;
        jsonBook["name"] = book.name;
        jsonBook["genre"] = book.genre;
        jsonBook["author"] = book.author;
        jsonBook["dueDate"] = book.dueDate;
        jsonOverdue.push_back(jsonBook);
    }
    return jsonOverdue;
}

// Function to deserialize JSON data to a data structure.
LibraryDatabase DeserializeLibraryFromJson(const json& jsonLibrary) {
    LibraryDatabase library;
    for (const auto& jsonBook : jsonLibrary) {
        Book book;
        book.name = jsonBook.value("name", "");
        book.genre = jsonBook.value("genre", "");
        book.dueDate = jsonBook.value("dueDate", "");
        library.books.push_back(book);
    }
    return library;
}

// Function to deserialize JSON data to a data structure.
OutsideLibraryDatabase DeserializeOutsideLibraryFromJson(const json& jsonLibrary) {
    OutsideLibraryDatabase outsideLibrary;
    for (const auto& jsonBook : jsonLibrary) {
        Book book;
        book.name = jsonBook.value("name", "");
        book.genre = jsonBook.value("genre", "");
        book.author = jsonBook.value("author", "");
        book.dueDate = jsonBook.value("dueDate", "");
        outsideLibrary.outsideBooks.push_back(book);
    }
    return outsideLibrary;
}

// Function to deserialize JSON data to a data structure.
OverdueDatabase DeserializeOverdueFromJson(const json& jsonOverdue) {
    OverdueDatabase overdue;
    for (const auto& jsonBook : jsonOverdue) {
        Book book;
        book.name = jsonBook.value("name", "");
        book.genre = jsonBook.value("genre", "");
        book.author = jsonBook.value("author", "");
        book.dueDate = jsonBook.value("dueDate", "");
        overdue.overdueBooks.push_back(book);
    }
    return overdue;
}

// Function to save library data to a JSON file.
void SaveLibraryData(const LibraryDatabase& library, const std::string& filename) {
    json jsonLibrary = SerializeToJson(library);
    std::ofstream file(filename);
    if (file.is_open()) {
        file << jsonLibrary.dump(4); // Dump with indentation for readability.
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
}

// Function to save outside library data to a JSON file.
void SaveOutsideLibraryData(const OutsideLibraryDatabase& outsideLibrary, const std::string& filename) {
    json jsonLibrary = SerializeToJson(outsideLibrary);
    std::ofstream file(filename);
    if (file.is_open()) {
        file << jsonLibrary.dump(4); // Dump with indentation for readability.
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
}

// Function to save overdue data to a JSON file.
void SaveOverdueData(const OverdueDatabase& overdue, const std::string& filename) {
    json jsonOverdue = SerializeToJson(overdue);
    std::ofstream file(filename);
    if (file.is_open()) {
        file << jsonOverdue.dump(4); // Dump with indentation for readability.
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
    }
}

// Function to load library data from a JSON file.
LibraryDatabase LoadLibraryData(const std::string& filename) {
    json jsonLibrary;
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> jsonLibrary;
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
    }
    return DeserializeLibraryFromJson(jsonLibrary);
}

// Function to load outside library data from a JSON file.
OutsideLibraryDatabase LoadOutsideLibraryData(const std::string& filename) {
    json jsonLibrary;
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> jsonLibrary;
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
    }
    return DeserializeOutsideLibraryFromJson(jsonLibrary);
}

// Function to load overdue data from a JSON file.
OverdueDatabase LoadOverdueData(const std::string& filename) {
    json jsonOverdue;
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> jsonOverdue;
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
    }
    return DeserializeOverdueFromJson(jsonOverdue);
}

// Function to get the current date in the format "YYYY-MM-DD".
std::string GetCurrentDate() {
    time_t now;
    struct tm timeinfo;
    char buffer[11]; // "YYYY-MM-DD\0"

    time(&now);

    if (localtime_s(&timeinfo, &now) == 0) {
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
        return std::string(buffer);
    }
    else {
        return "Error: Unable to retrieve the current date.";
    }
}

void ViewLibraryDatabase(const LibraryDatabase& library) {
    std::cout << "Library Database:\n";
    for (const auto& book : library.books) {
        std::cout << "Name: " << book.name << "\n";
        std::cout << "Genre: " << book.genre << "\n";
        std::cout << "Author: " << book.author << "\n";
        std::cout << "Due Date: " << book.dueDate << "\n";
        std::cout << "-------------------\n";
    }
}

void ViewOutsideLibraryDatabase(const OutsideLibraryDatabase& outsideLibrary) {
    std::cout << "Outside Library Database:\n";
    for (const auto& book : outsideLibrary.outsideBooks) {
        std::cout << "Name: " << book.name << "\n";
        std::cout << "Genre: " << book.genre << "\n";
        std::cout << "Author: " << book.author << "\n";
        std::cout << "Due Date: " << book.dueDate << "\n";
        std::cout << "-------------------\n";
    }
}

void ViewOverdueDatabase(const OverdueDatabase& overdue) {
    std::cout << "Overdue Database:\n";
    for (const auto& book : overdue.overdueBooks) {
        std::cout << "Name: " << book.name << "\n";
        std::cout << "Genre: " << book.genre << "\n";
        std::cout << "Author: " << book.author << "\n";
        std::cout << "Due Date: " << book.dueDate << "\n";
        std::cout << "-------------------\n";
    }
}

void CheckAndMoveOverdueBooks(OutsideLibraryDatabase& outsideLibrary, OverdueDatabase& overdue) {
    std::string currentDate = GetCurrentDate();
    MoveOverdueBooks(outsideLibrary, overdue, currentDate);
}

int main() {
    // Initialize library, outside-library, and overdue databases.
    LibraryDatabase library;
    OutsideLibraryDatabase outsideLibrary;
    OverdueDatabase overdue;

    // Load existing data from JSON files (if they exist).
    library = LoadLibraryData("library_data.json");
    outsideLibrary = LoadOutsideLibraryData("outside_library_data.json");
    overdue = LoadOverdueData("overdue_data.json");

    // Main menu loop (you can create a more user-friendly interface).
    while (true) {
        CheckAndMoveOverdueBooks(outsideLibrary, overdue); //every time the main function runs, checkandmoveoverduebooks will now be called if the due dates of the books out of the library have passed.
        int choice;
        std::cout << "Library Management Menu:\n";
        std::cout << "1. Add a book to the library\n";
        std::cout << "2. Remove a book from the library\n";
        std::cout << "3. Add a book to the outside library\n";
        std::cout << "4. Move overdue books from outside library\n";
        std::cout << "5. View Library Database\n";
        std::cout << "6. View Outside Library Database\n";
        std::cout << "7. View Overdue Database\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            Book newBook;
            std::cout << "Enter book name: ";
            std::cin.ignore();
            std::getline(std::cin, newBook.name);
            std::cout << "Enter book genre: ";
            std::getline(std::cin, newBook.genre);
            std::cout << "Enter the authors name: ";
            std::getline(std::cin, newBook.author);

            AddBookToLibrary(library, newBook);
            std::cout << "Book added to the library." << std::endl;
            break;
        }
        case 2: {
            std::string bookName;
            std::cout << "Enter the name of the book to remove from the library: ";
            std::cin.ignore();
            std::getline(std::cin, bookName);
            RemoveBookFromLibrary(library, bookName);
            std::cout << "Book removed from the library." << std::endl;
            break;
        }
        case 3: {
            Book newBook;
            std::cout << "Enter book name: ";
            std::cin.ignore();
            std::getline(std::cin, newBook.name);
            std::cout << "Enter book genre: ";
            std::getline(std::cin, newBook.genre);
            std::cout << "Enter the name of the person taking the book: ";
            std::getline(std::cin, newBook.author);
            std::cout << "Enter due date (YYYY-MM-DD): ";
            std::cin >> newBook.dueDate;

            AddBookToOutsideLibrary(outsideLibrary, newBook);
            std::cout << "Book added to the outside library." << std::endl;
            break;
        }
        case 4: {
            std::string currentDate = GetCurrentDate();
            MoveOverdueBooks(outsideLibrary, overdue, currentDate);
            std::cout << "Overdue books moved from the outside library." << std::endl;
            break;
        }
        case 5: {
            ViewLibraryDatabase(library);
            break;
        }
        case 6: {
            ViewOutsideLibraryDatabase(outsideLibrary);
            break;
        }
        case 7: {
            ViewOverdueDatabase(overdue);
            break;
        }
        case 8:
            // Save data to JSON files before exiting.
            SaveLibraryData(library, "library_data.json");
            SaveOutsideLibraryData(outsideLibrary, "outside_library_data.json");
            SaveOverdueData(overdue, "overdue_data.json");
            return 0;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}
