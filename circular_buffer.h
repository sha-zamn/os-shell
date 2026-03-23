#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class CircularBuffer {
private:
    vector<string> buffer;
    size_t capacity; // MAX_HISTORY
    size_t in;  //next element
    size_t out;  //oldest stored command
    size_t count;
    size_t total_added; 

public:
    // constructor to pre-allocate fixed-size array
    CircularBuffer(size_t max_capacity = 10) 
        : capacity(max_capacity), in(0), out(0), count(0), total_added(0) {
        buffer.resize(capacity);
    }

    // add command, overwrite oldest if full
    void add_cmd(const string& cmd) {
        if (cmd.empty()) return;
        
        if (count == capacity) {
            //if buffer full, evict oldest 
            out = (out + 1) % capacity;
        } else {
            count++;
        }
        
        buffer[in] = cmd;
        in = (in + 1) % capacity;
        total_added++;
    }

    string get_cmd(size_t cmd_no) const {
        if (cmd_no == 0 || cmd_no > count) {
            return "";
        }
        
        size_t current_command = cmd_no - 1;
        
        //convert to buffer index
        size_t index = ((in - 1 - current_command + capacity) % capacity);
        return buffer[index];
    }

    //Get most recent command
    string back() const {
        if (count == 0) return "";
        size_t index = ((in - 1) + capacity) % capacity;
        return buffer[index];
    }

    size_t total_commands() const {
        return total_added;
    }

    size_t size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    size_t get_capacity() const {
        return capacity;
    }

    // display history with numbering
    void display_history() const {
        if (empty()) {
            cout << "No commands in history." << endl;
            return;
        }

        // First displayed number = oldest stored command number
        size_t start_num = total_added - count + 1;
        
        for (size_t i = 0; i < count; i++) {
            size_t index = (out + i) % capacity;
            cout << (start_num + i) << " " << buffer[index] << endl;
        }
    }

    // Clear all history
    void clear() {
        in = 0;
        out = 0;
        count = 0;
        total_added = 0;
    }
};

#endif