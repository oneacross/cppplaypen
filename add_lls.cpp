
#include <iostream>
#include <memory>

using namespace std;

// Write a function that adds numbers represented as linked lists.
// The numbers are decimal, there is one digit per node, and the least
// significant digit is the head of the list.
//
// Observation:
// This is not working well with smart pointers.
// I'm guessing this is because the unique_ptr was only managing the memory for
// the head as the list grew. The compiler got confused, only knew how to
// delete the head, then all the other nodes leaked.
// BUT, still had a problem with simply constructing and iterating through a
// linked list. Dang. Stay away from smart pointers when creating your own data
// structures? Look at STL vector for an example.
//
// Key concept of c++14 smart pointers that I just got!
// When allocating memory, think of whether it should go on the heap or the
// stack. It should go on the heap if you want to return if from a function
// (like a factory). You should declare and transfer ownership with a
// unique_ptr.
// For a linked list, or any other type of data structure, remember:
// * Need to create the node on the heap, using make_unique.
// * ALSO need to release ownership after attaching the node to the data
// structure! Otherwise, the unique_ptr will destruct the node when the
// function exits. Soooo precise!!!!

// The linked list.
class Node {
    public:
    int value;

    // Use a raw pointer or unique_ptr for the next pointer?
    // Does this node manage the next nodes memory?
    // No, not really, it is just a reference. The ownership
    // is in the program using the structure.
    // Internal data structure references can remain raw pointers.
    // Also, using a raw pointer will save space.
    // And if you use a unique_ptr you cannot do simple node = node->next.
    Node* next;

    Node(int value = 0) : value(value) {}
};

// A function that converts an integer into a linked list.
unique_ptr<Node> int_to_list(int n) {

    // Should I create the first node before entering the loop?
    // Probably not, since it breaks an invariant.
    // Create a root node that points to the list!
    // Maybe this should just be a Node, create the unique_ptr
    // when returning the value.
    // Every node created must live on the heap, except for the root node that
    // is pointing to the list.
    auto ptr_to_list = Node();

    // Create a raw pointer to keep track of the tail.
    Node* tail = &ptr_to_list;

    // Handle the n = 0 case.
    if (n == 0) {
        ptr_to_list.value = 0;
        unique_ptr<Node> retptr(&ptr_to_list);
        return retptr;
    }

    while (n > 0) {
        // Create a new node on the heap with the next least significant digit.
        auto least_sig_digit = n % 10;
        unique_ptr<Node> new_node = make_unique<Node>(least_sig_digit);
        //cout << "Adding node for " << least_sig_digit << endl;

        // Append the new node to the list.
        tail->next = new_node.get();
        // Release the ownership of the new node.
        new_node.release();

        // Move the tail forward.
        tail = tail->next;

        // Integer divide to reduce n.
        n /= 10;
    }

    // Create ownership of the head of the list.
    // Feels strange to create the ownership here.
    unique_ptr<Node> retptr(ptr_to_list.next);

    return retptr;
}

// A function that prints out the list in a nice format.
void print_list(Node* list) {

    auto count = 0;

    while (list != nullptr) {
        cout << list->value;

        if (list->next) {
            cout << " -> ";
        }

        list = list->next;

        count += 1;
//        cout << count << " iterations through loop" << endl;
    }
    cout << endl;
}

auto create_list() {
    // Create and return a 2 node linked list.
    // This means the list must exist on the heap.
    unique_ptr<Node> node1 = make_unique<Node>(3);
    unique_ptr<Node> node2 = make_unique<Node>(17);

    // Add the new node to the list.
    node1->next = node2.get();
    // Release the new node's ownership.
    node2.release();

    // Transfer ownership of the linked list.
    return node1;
}

// Sum two intlists and return as another intlist.
auto add_intlists(Node* intlist1, Node* intlist2) {

    unique_ptr<Node> ptr_to_sumlist = make_unique<Node>();
    Node* sumlist_tail = ptr_to_sumlist.get();
    bool carry = false;

    while (intlist1 && intlist2) {

        int sum = intlist1->value + intlist2->value + carry;
        carry = sum > 9;
        int digit = sum % 10;

        // Create a new node and append.
        unique_ptr<Node> new_node = make_unique<Node>(digit);
        sumlist_tail->next = new_node.get();
        // Release the ownership.
        new_node.release();

        // Forward the tail.
        sumlist_tail = sumlist_tail->next;

        intlist1 = intlist1->next;
        intlist2 = intlist2->next;
    }

    // If intlist1 is longer than intlist2.
    while (intlist1) {
        int sum = intlist1->value + carry;
        carry = sum > 9;
        int digit = sum % 10;

        // Create a new node and append.
        unique_ptr<Node> new_node = make_unique<Node>(digit);
        sumlist_tail->next = new_node.get();
        // Release the ownership.
        new_node.release();

        // Forward the tail.
        sumlist_tail = sumlist_tail->next;

        intlist1 = intlist1->next;
    }

    // If intlist2 is longer than intlist1.
    while (intlist2) {
        int sum = intlist2->value + carry;
        carry = sum > 9;
        int digit = sum % 10;

        // Create a new node and append.
        unique_ptr<Node> new_node = make_unique<Node>(digit);
        sumlist_tail->next = new_node.get();
        // Release the ownership.
        new_node.release();

        // Forward the tail.
        sumlist_tail = sumlist_tail->next;

        intlist2 = intlist2->next;
    }

    // Check for a final carry.
    if (carry) {
        // Create a new node for the final carry.
        unique_ptr<Node> new_node = make_unique<Node>(1);
        sumlist_tail->next = new_node.get();
        // Release the ownership.
        new_node.release();

        // Forward the tail.
        sumlist_tail = sumlist_tail->next;
    }

    // Convert the head of the list to a unique_ptr
    // to put the node on the heap.
    unique_ptr<Node> sumlist(ptr_to_sumlist->next);

    return sumlist;
}

int main() {
    auto num1 = 1729, num2 = 12;
    cout << "Adding " << num1 << " and " << num2 << endl;
    auto intlist1 = int_to_list(num1);
    auto intlist2 = int_to_list(num2);
    auto sumlist = add_intlists(intlist1.get(), intlist2.get());
    print_list(sumlist.get());
}

