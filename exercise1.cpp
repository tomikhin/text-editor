#include <iostream>
#include <string>
#include <algorithm> // required in order to implement toLowerCase function (namely to use std::transform)
#include "mystack.h" // Stack header file from the course

struct operation {
    enum Type { INSERT, DELETE } type; // type of operation (insert or delete)
    int index; // index from which the operation starts
    std::string text; // text to be inserted or deleted
};


std::string main_text = ""; //all operations apply to this text

// function to convert a string to lowercase
std::string toLowerCase(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

// function to insert a string based on starting index in the main text
void insertStr (int insertion_index, std::string insertion_text, Stack<operation> &undoStack, Stack<operation> &redoStack) 
{
    main_text.insert(insertion_index, insertion_text);
    struct operation insertion_op;
    insertion_op.type = operation::INSERT;
    insertion_op.index = insertion_index;
    insertion_op.text = insertion_text;

    undoStack.push(insertion_op);

    // clear the redo stack after a new operation is performed (otherwise the logical chain of commands is lost)
    while (!redoStack.isEmpty()) 
    {
        redoStack.pop();
    }
}

// function to delete substring based on starting index and the length of the substring in the main text
void deleteStr (int deletion_index, int deletion_length, Stack<operation> &undoStack, Stack<operation> &redoStack) 
{
    std::string deleted_str = main_text.substr(deletion_index, deletion_length); //save deleted substring in order to push it in the undoStack later
    main_text = main_text.erase(deletion_index, deletion_length);
    struct operation deletion_op;
    deletion_op.type = operation::DELETE;
    deletion_op.index = deletion_index;
    deletion_op.text = deleted_str;

    undoStack.push(deletion_op);

    // clear the redo stack after a new operation is performed (otherwise the logical chain of commands is lost)
    while (!redoStack.isEmpty()) 
    {
        redoStack.pop();
    }
}

void undo(Stack<operation> &undoStack, Stack<operation> &redoStack) 
{
    if (undoStack.isEmpty()) 
    {
        std::cout << "The UNDO operation cannot be done. There is nothing to undo." << std::endl;
    }
    else 
    {
        struct operation undone_op = undoStack.pop();
        if (undone_op.type == operation::INSERT) 
        {
            main_text.erase(undone_op.index, undone_op.text.length()); // erase the inserted substring from the main text
        }
        else 
        {
            main_text.insert(undone_op.index, undone_op.text); // insert the deleted substring back into the main text
        }
        redoStack.push(undone_op);
    }
}

void redo(Stack<operation> &undoStack, Stack<operation> &redoStack)
{
    if (redoStack.isEmpty()) 
    {
        std::cout << "The REDO operation cannot be done. There is nothing to redo." << std::endl;
    }
    else
    {
        struct operation redone_op = redoStack.pop();
        if (redone_op.type == operation::INSERT)
        {
            main_text.insert(redone_op.index, redone_op.text);
        }
        else
        {
            main_text.erase(redone_op.index, redone_op.text.length());
        }
        undoStack.push(redone_op);
    }
}

// so if to undo something, apply the opposite operation (e.g. if we inserted something, we delete it and vice versa)
// if to redo something, apply the same operation as it was before the undo operation

int main()
{
    std::string command;
    std::string sequence;
    Stack<operation> undoStack;
    Stack<operation> redoStack;

    std::cout << "Big Homework 1 - Exercise 1" << std::endl;
    std::cout << "\nAvailable commands:" << std::endl;
    std::cout << "INSERT index_of_insertion text_to_insert" << std::endl;
    std::cout << "DELETE index_of_deletion the_length_of_the_substring_to_be_deleted" << std::endl;
    std::cout << "UNDO (revert the last operation)" << std::endl;
    std::cout << "REDO (reapply an operation that was undone)" << std::endl;
    std::cout << "EXIT (to end the execution of the program)" << std::endl;

    while (true)
    {
        std::cout << "\nEnter a command: " << std::endl;
        std::getline(std::cin, sequence);

        int index_found = sequence.find(" ");

        if (index_found != std::string::npos)
        {
            command = toLowerCase(sequence.substr(0, index_found));
        }
        else
            command = toLowerCase(sequence);

        if (command == "insert")
        {
            int index_found_next = sequence.find(" ", index_found + 1);
            if (index_found_next == std::string::npos)
            {
                std::cout << "Invalid command." << std::endl;
                continue;
            }
            try //if str_position is not a number (etc letter) it will throw an exception
            {
                int str_position = std::stoi(sequence.substr(index_found + 1, index_found_next - index_found - 1));
                std::string str_text = sequence.substr(index_found_next + 1);
                if (str_position < 0)
                {
                    std::cout << "Error - The index of insertion cannot be negative." << std::endl; //
                    continue;
                }
                else if (str_position > main_text.length())
                {
                    std::cout << "Error - It is not possible to insert string beyond the text." << std::endl; //
                    continue;
                }
                insertStr(str_position, str_text, undoStack, redoStack);
            }
            catch (...)
            {
                std::cout << "Error - The index of insertion has to be a number." << std::endl;
                continue;
            }

        }
        else if (command == "delete")
        {
            int index_found_next = sequence.find(" ", index_found + 1);
            if (index_found_next == std::string::npos)
            {
                std::cout << "Invalid command." << std::endl;
                continue;
            }
            try //if str_position and delete_length are not numbers (etc letters) it will throw an exception
            {
                int str_position = std::stoi(sequence.substr(index_found + 1, index_found_next - index_found - 1));
                int delete_length = std::stoi(sequence.substr(index_found_next + 1));
                if (str_position < 0)
                {
                    std::cout << "Error - The index of deletion cannot be negative." << std::endl; //
                    continue;
                }
                else if (str_position >= main_text.length())
                {
                    std::cout << "Error - It is not possible to delete a substring beyond the text." << std::endl; //
                    continue;
                }
                deleteStr(str_position, delete_length, undoStack, redoStack);
            }
            catch (...)
            {
                std::cout << "Error - the index of deletion and the length of the substring have to be a number." << std::endl;
                continue;
            }
            
        }
        else if (command == "undo")
        {
            undo(undoStack, redoStack);
        }
        else if (command == "redo")
        {
            redo(undoStack, redoStack);
        }
        else if (command == "exit")
        {
            return 0;
        }
        else
        {
            std::cout << "Invalid command." << std::endl;
            continue;
        }

        std::cout << "Main text: " << main_text << std::endl;
    }
    return 0;
}
    