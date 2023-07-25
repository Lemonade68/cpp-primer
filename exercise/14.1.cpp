//练习14.5

#ifndef TEST_BOOK_H
#define TEST_BOOK_H

//.h
#include <iostream>
#include <string>

class Book {
    friend std::istream &operator>>(std::istream&, Book&);
    friend std::ostream &operator<<(std::ostream&, const Book&);
    friend bool operator==(const Book&, const Book&);
    friend bool operator!=(const Book&, const Book&);

public:
    Book() = default;
    Book(std::string no, std::string name, std::string author, std::string pubdate)
        : no_(no), name_(name), author_(author), pubdate_(pubdate) { }
    Book(std::istream &in) { in >> *this; }

private:
    std::string no_;
    std::string name_;
    std::string author_;
    std::string pubdate_;
};

std::istream &operator>>(std::istream&, Book&);
std::ostream &operator<<(std::ostream&, const Book&);
bool operator==(const Book&, const Book&);
bool operator!=(const Book&, const Book&);




//.cpp
std::istream &operator>>(std::istream &in, Book &book) {
    in >> book.no_ >> book.name_ >> book.author_ >> book.pubdate_;
    //先全输入完后再一次性判断是否正确
    if (!in)
        book = Book();
    return in;
}

std::ostream &operator<<(std::ostream &out, const Book &book) {
    out << book.no_ << " " << book.name_ << " " << book.author_
        << " " << book.pubdate_;
    return out;
}

bool operator==(const Book &lhs, const Book &rhs) {
    return lhs.no_ == rhs.no_;
}

bool operator!=(const Book &lhs, const Book &rhs) {
    return !(lhs == rhs);
}


#endif //TEST_BOOK_H
