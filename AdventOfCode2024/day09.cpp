//
// Created by Sull on 11/27/20
#include <iostream>
#include <list>

#include "utils/input_reader.h"

using namespace std;

class FilePointer {
public:
    int index; //references input file
    int capacity; //value from input file
    int value; //final index

    FilePointer(int index, int capacity, int value = 0) : index(index), capacity(capacity), value(value) {}
    FilePointer(int index, char capacity, int value = 0) {
        this->index = index;
        this->capacity = capacity-'0';
        this->value = value;
    }

    void moveForward(const string& input) {
        index++;
        capacity = input[index] - '0';
        if (this->isValidFile()) value++;
    }

    void moveBackward(const string& input) {
        index = index - 2;
        capacity = input[index] - '0';
        value--;
    }

    bool isValidFile() {
        if (index%2==0) return true;
        return false;
    }

    bool isValidCapacity() {
        if (this->capacity <= 0) return false;
        return true;
    }
};

int calculateOutputLength(string& input) {
    int sum = 0;
    for (int i=0; i<input.length(); i+=2) {
        sum += input[i] - '0';
    }
    return sum;
}

//PART 2
struct Segment {
    int length;
    bool isFile;
    int id = -1;
};

class FileSystem {
    public:
    list<Segment> segments;

    FileSystem(const string& input) {
        int id = 0;
        for (int i=0; i<input.length(); i++) {
            if (i%2==0) {
                segments.emplace_back(Segment(
                input[i]-'0',
                true,
                id));
                id++;
            } else {
                segments.emplace_back(input[i]-'0', false);
            }
        }
    }

    void arrangeSegments() {
        std::list<Segment>::reverse_iterator backward_iterator = segments.rbegin();

        for (; backward_iterator != segments.rend(); ++backward_iterator) {
            if (backward_iterator->isFile) {
                std::list<Segment>::const_iterator cit = prev(backward_iterator.base());
                findPlace(backward_iterator->length, cit);
            }
        }
    }

    void findPlace(int length, auto it) {
        auto forward_iterator = segments.begin();
        for (; forward_iterator != it; ++forward_iterator) {
            if (!forward_iterator->isFile && forward_iterator->length >= length) {
                segments.insert(it, Segment(length, false));
                segments.splice(forward_iterator, segments, it);
                forward_iterator->length -= length;
                return;
            }
        }
    }

    long long calculateChecksum() {
        long long sum = 0;
        int index = 0;
        for (auto it = segments.begin(); it != segments.end(); ++it) {
            if (it->isFile) {
                for (int i=0; i<it->length; ++i) {
                    sum += it->id * index;
                    index++;
                }
            } else {
                for (int i=0; i<it->length; ++i, ++index) {}
            }
        }

        return sum;
    }

    //helper for debugging
    void printSegments() {
        for (Segment& segment : segments) {
            if (segment.isFile) {
                for (int i=0; i<segment.length; ++i) cout<<segment.id;
            } else for (int i=0; i<segment.length; ++i) cout<<".";
        }
        cout<<endl;
    }
};

int main() {
    string input = readString("data/day9_input.txt");
    //string input = "2333133121414131402";

    //PART 1
    /*
    int length = input.length();
    FilePointer front(0, input[0]);
    FilePointer back(length-1, input[length-1], (int)length/2);
    int output_length = calculateOutputLength(input);

    long long checksum = 0;

    int i = 0;
    while (i < output_length) {
        if (front.isValidFile()) {
            while (front.isValidCapacity()) {
                cout<<front.value;
                checksum += front.value * i;
                front.capacity--;
                if (++i >= output_length) break;
            }
            front.moveForward(input);
        } else {
            while (front.isValidCapacity()) {
                cout<<back.value;
                checksum += back.value * i;
                front.capacity--;
                if (++i >= output_length) break;
                if (--back.capacity <= 0) back.moveBackward(input);
            }
            front.moveForward(input);
        }
    }

    cout<<"\nChecksum: "<<checksum<<endl;
    */

    //PART 2
    FileSystem fs(input);
    fs.arrangeSegments();
    cout <<"Checksum: "<< fs.calculateChecksum() << endl;

    return 0;
}