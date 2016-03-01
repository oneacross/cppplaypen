
#include <iostream>
#include <memory>

using namespace std;

class Node {
    public:
        int value;
        // Test: Use an uninitialized unique_ptr.
        unique_ptr<Node> next;

        Node(int value = 0) : value(value) {}
};

int main() {
    Node n = Node(3);

    if (n.next == nullptr) {
        cout << "is a nullptr!" << endl;
    }

    // Objective: Create a one node linked list in a function and return it to
    // the caller as a unique_ptr.
    // Possibly there is more memory than the unique_ptr knows how to delete.
    // Lots of nodes dangling?
    // How to delete a linked list?
    // I'd prefer not to think about that, maybe just create a list of
    // unique_ptrs?
}

void back_to_basics() {
    cout << "Back to basics" << endl;

    // Step 1, create a unique_ptr to a Node and pass it to another node.
    unique_ptr<Node> n1 = make_unique<Node>(3);
    cout << "n1 value is " << n1->value << endl;

    // Now n2 owns the node.
    unique_ptr<Node> n2(move(n1));
    cout << "n2 value is " << n2->value << endl;

    // What does this mean? Can I still access the value through n1?
    // Nope! This causes a segfault! Once ownership has been moved,
    // thou shalt not touch the past owner!
    //cout << "n1 value is " << n1->value << endl;
}

