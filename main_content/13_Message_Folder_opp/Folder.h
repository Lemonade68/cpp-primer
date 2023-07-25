#ifndef TEST_FOLDER_H
#define TEST_FOLDER_H

#include <string>
using std::string;

#include <set>
using std::set;

class Folder;                   

class Message {
    friend void swap(Message&, Message&);
    friend class Folder;

public:
    // folders is implicitly initialized to the empty set
    explicit Message(const string &str = "") : contents(str) { }

    // copy control to manage pointers to this Message
    Message(const Message&);                // copy constructor
    Message& operator=(const Message&);     // copy assignment
    ~Message();                             // destructor

    // add/remove this Message from the specified Folder's set of messages
    void save(Folder&);
    void remove(Folder&);
    void debug_print();             // print contents and it's list of Folders,
    // printing each Folder as well

private:
    string contents;                // actual message text
    set<Folder*> folders;           // Folders that have this Message

    // utility functions used by copy constructor, assignment, and destructor
    // add this Message to the Folders that point to the parameter
    void add_to_Folders(const Message&);

    // remove this Message from every Folder in folders
    void remove_from_Folders();

    // used by Folder class to add self to this Message's set of Folder's
    void addFldr(Folder *f) { folders.insert(f); }
    void remFldr(Folder *f) { folders.erase(f); }
};
// declaration for swap should be in the same header as Message itself
void swap(Message&, Message&);

class Folder {
    friend void swap(Message&, Message&);
    friend class Message;

public:
    ~Folder();                              // remove itself from Message in msgs
    Folder(const Folder&);                  // add new folder to each Message in msgs
    Folder& operator=(const Folder&);       // delete Folder from lhs messages
    // add Folder to rhs messages
    Folder() { }                            // defaults ok

    void save(Message&);                    // add this message to folder
    void remove(Message&);                  // remove this message from this folder

    void debug_print();                     // print contents and it's list of Folders

private:
    set<Message*> msgs;                     // messages in this folder

    void add_to_Messages(const Folder&);    // add this Folder to each Message
    void remove_from_Msgs();                // remove this Folder from each Message
    void addMsg(Message *m) { msgs.insert(m); }
    void remMsg(Message *m) { msgs.erase(m); }
};

#include <iostream>
using std::cerr; using std::endl;

#include <set>
using std::set;

#include <string>
using std::string;


void swap(Message &lhs, Message &rhs) {
    using std::swap;        // not strictly needed in this case, but good habit

    // remove pointers to each Message from their (original) respective Folders
    for (set<Folder*>::iterator f = lhs.folders.begin();
         f != lhs.folders.end(); ++f)
        (*f)->remMsg(&lhs);
    for (set<Folder*>::iterator f = rhs.folders.begin();
         f != rhs.folders.end(); ++f)
        (*f)->remMsg(&rhs);

    // swap the contents and Folder pointer sets
    swap(lhs.folders, rhs.folders);         // uses swap(set&, set&)
    swap(lhs.contents, rhs.contents);       // swap(string&, string&)

    // add pointers to each Message to their (new) respective Folders
    for (set<Folder*>::iterator f = lhs.folders.begin();
         f != lhs.folders.end(); ++f)
        (*f)->addMsg(&lhs);
    for (set<Folder*>::iterator f = rhs.folders.begin();
         f != rhs.folders.end(); ++f)
        (*f)->addMsg(&rhs);
}

Message::Message(const Message &m) : contents(m.contents), folders(m.folders) {
    add_to_Folders(m);          // add this Message to the Folders that point to m
}

Message& Message::operator=(const Message &rhs) {
    // handle self-assignment by removing pointers before inserting them
    remove_from_Folders();      // update existing Folders
    contents = rhs.contents;    // copy message contents from rhs
    folders = rhs.folders;      // copy Folder pointers from rhs
    add_to_Folders(rhs);        // add this Message to those Folders
    return *this;
}

Message::~Message() {
    remove_from_Folders();
}

// add this Message to Folders that point to m
void Message::add_to_Folders(const Message &m) {
    for (set<Folder*>::iterator f = m.folders.begin();
         f != m.folders.end(); ++f)         // for each Folder that holds m
        (*f)->addMsg(this);         // add a pointer to this Message to that Folder
}

// remove this Message from the corresponding Folders
void Message::remove_from_Folders() {
    for (set<Folder*>::iterator f = folders.begin();
         f != folders.end(); ++f)           // for each pointer in folders
        (*f)->remMsg(this);         // remove this Message from that Folder
}

void Folder::add_to_Messages(const Folder &f) {
    for (set<Message*>::iterator msg = f.msgs.begin();
         msg != f.msgs.end(); ++msg)
        (*msg)->addFldr(this);      // add this Folder to each Message
}

Folder::Folder(const Folder &f) : msgs(f.msgs) {
    add_to_Messages(f);         // add this Folder to each Message in f.msgs
}

Folder& Folder::operator=(const Folder &f) {
    remove_from_Msgs();         // remove this folder from each Message in msgs
    msgs = f.msgs;              // copy the set of Message from f
    add_to_Messages(f);         // add this folder to each Message in msgs
    return *this;
}

Folder::~Folder() {
    remove_from_Msgs();
}

void Folder::remove_from_Msgs() {
    while (!msgs.empty())
        (*msgs.begin())->remove(*this);
}
void Message::save(Folder &f) {
    folders.insert(&f);         // add the given Folder to our list of Folders
    f.addMsg(this);             // add this Message to f's set of Messages
}

void Message::remove(Folder &f) {
    folders.erase(&f);          // take the given Folder out of our list of Folders
    f.remMsg(this);             // remove this Message to f's set of Messages
}

void Folder::save(Message &m) {
    // add m and add this folder to m's set of Folders
    msgs.insert(&m);
    m.addFldr(this);
}

void Folder::remove(Message &m) {
    // erase m from msgs and remove this folder from m
    msgs.erase(&m);
    m.remFldr(this);
}

void Folder::debug_print() {
    cerr << "Folder contains " << msgs.size() << " messages" << endl;
    int ctr = 1;
    for (set<Message*>::iterator m = msgs.begin();
         m != msgs.end(); ++m) {
        cerr << "Message " << ctr++ << ":\n\t" << (*m)->contents << endl;
    }
}

void Message::debug_print() {
    cerr << "Message:\n\t" << contents << endl;
    cerr << "Appears in " << folders.size() << " Folders" << endl;
}


#endif //TEST_FOLDER_H
