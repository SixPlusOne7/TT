#include <emscripten.h>
#include <sstream>
#include <cstring>
#include "llist.h"

using namespace std;

// Global variables for WebAssembly
static llist* database = nullptr;
static stringstream web_output;
static string current_filename = "webdata.txt";

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE
void init_database() {
    if(database == nullptr) {
        database = new llist(const_cast<char*>(current_filename.c_str()));
    }
}

EMSCRIPTEN_KEEPALIVE
void process_command(const char* command) {
    if(database == nullptr) init_database();
    
    string cmd(command);
    web_output.str("");

    if(cmd.find("add") == 0) {
        int accountno;
        char name[25];
        char address[45];
        
        // Parse command: "add|accountno|name|address"
        sscanf(cmd.c_str(), "add|%d|%24[^|]|%44[^|]", &accountno, name, address);
        database->addRecord(accountno, name, address);
        web_output << "Added record for account #" << accountno;
    }
    else if(cmd.find("printall") == 0) {
        database->printAllRecords();
    }
    else if(cmd.find("find") == 0) {
        int accountno;
        sscanf(cmd.c_str(), "find|%d", &accountno);
        database->findRecord(accountno);
    }
    else if(cmd.find("delete") == 0) {
        int accountno;
        sscanf(cmd.c_str(), "delete|%d", &accountno);
        database->deleteRecord(accountno);
    }
    else {
        web_output << "Invalid command: " << cmd;
    }
}

EMSCRIPTEN_KEEPALIVE
const char* get_output() {
    static string output;
    output = web_output.str();
    return output.c_str();
}

#ifdef __cplusplus
}
#endif

// Modified print functions to use web_output
void llist::printAllRecords() {
    struct record* current = start;
    if(current == NULL) {
        web_output << "No records in database\n";
        return;
    }
    
    while(current != NULL) {
        web_output << "Account: " << current->accountno << "\n"
                   << "Name: " << current->name << "\n"
                   << "Address: " << current->address << "\n\n";
        current = current->next;
    }
}

// Remove original main function
