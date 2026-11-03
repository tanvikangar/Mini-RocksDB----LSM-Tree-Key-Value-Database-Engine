#include <iostream>
#include <sstream>
#include <string>
#include "database_engine.h"

void showHelp() {
    std::cout << "Commands:\n"
              << "  put <key> <value>   store a key and value\n"
              << "  get <key>           read a value\n"
              << "  delete <key>        delete a key\n"
              << "  flush               save the MemTable to a disk file now\n"
              << "  stats               show files created and Bloom Filter savings\n"
              << "  help                show this list\n"
              << "  exit                save and quit\n";
}

int main() 
{
    DatabaseEngine db("mydb", 1000);
    std::cout << "=== Mini RocksDB ===\n";
    showHelp();

    std::string line;
    while (true) 
    {
        std::cout << "\nminirocks> ";
        if (!std::getline(std::cin, line)) 
        {
            break;
        }

        std::istringstream input(line);
        std::string command, key, value;
        input >> command >> key;
        std::getline(input, value);
        if (!value.empty() && value[0] == ' ') 
        {
            value = value.substr(1);
        }

        if (command == "put") 
        {
            if (key.empty() || value.empty()) 
            {
                std::cout << "Usage: put <key> <value>\n";
            } 
            else if (key.find('|') != std::string::npos || value.find('|') != std::string::npos) 
            {
                std::cout << "Error: the character | is not allowed\n";
            } 
            else 
            {
                db.put(key, value);
                std::cout << "OK\n";
            }
        } 
        else if (command == "get") 
        {
            std::string result;
            if (key.empty()) 
            {
                std::cout << "Usage: get <key>\n";
            }
            else if (db.get(key, result)) 
            {
                std::cout << result << "\n";
            }
            else 
            {
                std::cout << "(not found)\n";
            }
        } 
        else if (command == "delete") 
        {
            if (key.empty()) 
            {
                std::cout << "Usage: delete <key>\n";
            }
            else 
            {
                db.remove(key);
                std::cout << "OK\n";
            }
        } 
        else if (command == "flush") 
        {
            db.flush();
            std::cout << "Saved. Files on disk: " << db.tableCount() << "\n";
        }
        else if (command == "stats") 
        {
            std::cout << "Files on disk:      " << db.tableCount() << "\n"
                      << "Skipped by Bloom:   " << db.bloomSkips << "\n"
                      << "Files really read:  " << db.diskReads << "\n";
        } 
        else if (command == "help") 
        {
            showHelp();
        } 
        else if (command == "exit" || command == "quit") 
        {
            break;
        } 
        else if (!command.empty()) 
        {
            std::cout << "Unknown command. Type help.\n";
        }
    }
    std::cout << "Saving and closing...\n";
    return 0;
}