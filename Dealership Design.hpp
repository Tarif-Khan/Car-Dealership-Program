#ifndef RESERVATION_SYSTEM_HPP
#define RESERVATION_SYSTEM_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>

using namespace std;

template<typename T>
class ReservationSystem {
private:
    struct Reservation {
        T item;
        string associatedInfo;
        string reserverName;
        string reserverDOB;
        int reserverId;
        chrono::system_clock::time_point reservationTime;
        Reservation(const T& item, const string& info, const string& name, const string& dob, int id)
            : item(item), associatedInfo(info), reserverName(name), reserverDOB(dob), reserverId(id),
              reservationTime(chrono::system_clock::now()) {}
    };
    vector<Reservation> reservations;

public:
    void reserve(const T& item, const string& info, const string& name, const string& dob, int id) {
        auto it = find_if(reservations.begin(), reservations.end(),
            [id](const Reservation& res) { return res.reserverId == id; });
        if (it != reservations.end()) {
            it->item = item;
            it->associatedInfo = info;
        } else {
            reservations.emplace_back(item, info, name, dob, id);
        }
    }

    void addItems(const vector<pair<T, string>>& items, const string& name, const string& dob, int id) {
        for (const auto& item : items) {
            reserve(item.first, item.second, name, dob, id);
        }
    }

    long long getReservationDuration(int id) const {
        auto it = find_if(reservations.begin(), reservations.end(),
            [id](const Reservation& res) { return res.reserverId == id; });
        if (it != reservations.end()) {
            auto duration = chrono::duration_cast<chrono::seconds>(
                chrono::system_clock::now() - it->reservationTime);
            return duration.count();
        }
        return -1;
    }

    void removeReservation(int id) {
        auto it = remove_if(reservations.begin(), reservations.end(),
            [id](const Reservation& res) { return res.reserverId == id; });
        reservations.erase(it, reservations.end());
    }

    void removeReservations(const vector<int>& ids) {
        for (int id : ids) {
            removeReservation(id);
        }
    }

    size_t getNumReservations() const {
        return reservations.size();
    }

    friend ostream& operator<<(ostream& os, const ReservationSystem& rs) {
        for (const auto& res : rs.reservations) {
            os << "Reservation ID: " << res.reserverId << "\n";
            os << "Item: " << res.item << "\n";
            os << "Associated Info: " << res.associatedInfo << "\n";
            os << "Reserver Name: " << res.reserverName << "\n";
            os << "Reserver DOB: " << res.reserverDOB << "\n";
            os << "Reservation Time: " << chrono::system_clock::to_time_t(res.reservationTime) << "\n\n";
        }
        return os;
    }

    friend istream& operator>>(istream& is, ReservationSystem& rs) {
        T item;
        string associatedInfo;
        string reserverName;
        string reserverDOB;
        int reserverId;

        string line;
        while (getline(is, line)) {
            if (line.empty()) {
                continue;
            }
            istringstream iss(line);
            string field;
            if (getline(iss, field, ':') && field == "Reservation ID") {
                iss >> reserverId;
            } else if (getline(iss, field, ':') && field == "Item") {
                iss >> item;
            } else if (getline(iss, field, ':') && field == "Associated Info") {
                getline(iss, associatedInfo);
            } else if (getline(iss, field, ':') && field == "Reserver Name") {
                getline(iss, reserverName);
            } else if (getline(iss, field, ':') && field == "Reserver DOB") {
                getline(iss, reserverDOB);
            }
        }
        rs.reserve(item, associatedInfo, reserverName, reserverDOB, reserverId);
        return is;
    }
};

#endif // RESERVATION_SYSTEM_HPP

// User-defined types for testing
struct Book {
    string title;
    string author;
    int year;
    friend ostream& operator<<(ostream& os, const Book& book) {
        os << "Title: " << book.title << ", Author: " << book.author << ", Year: " << book.year;
        return os;
    }
    friend istream& operator>>(istream& is, Book& book) {
        is >> book.title >> book.author >> book.year;
        return is;
    }
};

struct Concert {
    string artist;
    string venue;
    string date;
    friend ostream& operator<<(ostream& os, const Concert& concert) {
        os << "Artist: " << concert.artist << ", Venue: " << concert.venue << ", Date: " << concert.date;
        return os;
    }
    friend istream& operator>>(istream& is, Concert& concert) {
        is >> concert.artist >> concert.venue >> concert.date;
        return is;
    }
};

// Testing code
int main() {
    // Test with int
    ReservationSystem<int> intReservations;
    intReservations.reserve(10, "Table for 2", "John Doe", "1990-01-01", 1);
    intReservations.reserve(20, "Table for 4", "Jane Smith", "1985-05-15", 2);
    cout << "Int Reservations:\n" << intReservations << endl;

    // Test with string
    ReservationSystem<string> stringReservations;
    stringReservations.reserve("ABC123", "Economy Class", "Mike Johnson", "1992-09-10", 1);
    stringReservations.reserve("XYZ789", "Business Class", "Emily Davis", "1988-12-03", 2);
    cout << "String Reservations:\n" << stringReservations << endl;

    // Test with Book
    ReservationSystem<Book> bookReservations;
    bookReservations.reserve({"Book1", "Author1", 2021}, "Fiction", "David Wilson", "1995-07-20", 1);
    bookReservations.reserve({"Book2", "Author2", 2022}, "Non-Fiction", "Sarah Taylor", "1998-03-25", 2);
    cout << "Book Reservations:\n" << bookReservations << endl;

    // Test with Concert
    ReservationSystem<Concert> concertReservations;
    concertReservations.reserve({"Artist1", "Venue1", "2023-06-01"}, "VIP Tickets", "Daniel Brown", "1993-11-12", 1);
    concertReservations.reserve({"Artist2", "Venue2", "2023-07-15"}, "General Admission", "Olivia Anderson", "1997-02-28", 2);
    cout << "Concert Reservations:\n" << concertReservations << endl;

    // Test addItems
    vector<pair<Book, string>> bookItems = {
        {{"Book3", "Author3", 2023}, "Mystery"},
        {{"Book4", "Author4", 2024}, "Science Fiction"}
    };
    bookReservations.addItems(bookItems, "Emma Martinez", "1991-04-05", 3);
    cout << "Updated Book Reservations:\n" << bookReservations << endl;

    // Test getReservationDuration
    cout << "Reservation Duration for ID 1: " << bookReservations.getReservationDuration(1) << " seconds" << endl;
    cout << "Reservation Duration for ID 5: " << bookReservations.getReservationDuration(5) << " seconds" << endl;

    // Test removeReservation and removeReservations
    bookReservations.removeReservation(2);
    cout << "Book Reservations after removing ID 2:\n" << bookReservations << endl;

    concertReservations.removeReservations({1, 2});
    cout << "Concert Reservations after removing IDs 1 and 2:\n" << concertReservations << endl;

    // Test getNumReservations
    cout << "Number of Int Reservations: " << intReservations.getNumReservations() << endl;
    cout << "Number of String Reservations: " << stringReservations.getNumReservations() << endl;
    cout << "Number of Book Reservations: " << bookReservations.getNumReservations() << endl;
    cout << "Number of Concert Reservations: " << concertReservations.getNumReservations() << endl;

    return 0;
}