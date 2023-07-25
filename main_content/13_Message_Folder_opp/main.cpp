// #include "Folder.h"
#include "Message.h"

int main() {
    Message firstMail("hello");
    Message signInMail("welcome to cppprimer");
    Folder mailBox;

    firstMail.debug_print();        // print: "hello"
    firstMail.save(mailBox);        // send to mailBox
    mailBox.debug_print();          // print: "hello"

    signInMail.debug_print();       // print "welcome to cppprimer"
    signInMail.save(mailBox);       // send to mailBox
    mailBox.debug_print();          // print "welcome to cppprimer hello"

    firstMail = firstMail;          // test for assignment to self
    firstMail.debug_print();        // print "hello"
    mailBox.debug_print();          // print "welcome to cppprimer hello"

    return 0;
}