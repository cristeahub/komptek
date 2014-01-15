#include <stdio.h>
#include <stdlib.h>

#define SWAP(T, x, y) do { \
    T temp = x; \
    x = y; \
    y = temp; \
} while(0)

// A struct for tree nodes, with pointers to the parent and child nodes
// and a int to store the value at the node.
typedef struct Node{
    struct Node* left;
    struct Node* right;
    int value;
} Node;


// Returns a random number between 0 and n
// For simplicity, the random number generator is not seeded,
// hence the same random numbers will be generated on every execution
int get_random_number(int n){
    return rand() % n;
}


// Return a (dynamically allocated) array of length size,
// filled with random numbers between 0 and n
int* create_random_array(int size, int n){
    int *random_array = (int*)malloc(size * sizeof(int));

    for(int i = 0; i < size; i++) {
        random_array[i] = get_random_number(n);
    }

    return random_array;
}


// Should print the contents of array of lenght size
void print_array(int* array, int size){
    for(int i = 0; i < size; i++) {
        printf("%d\t", array[i]);
    }
    printf("\n");
}


// Should sort the numbers in array in increasing order
void sort(int* array, int size){
    // This is an implementation of insertion sort
    for(int i = 0; i < size; i++) {
        int j = i;
        while(j > 0 && array[j-1] > array[j]) {
            SWAP(int, array[j], array[j-1]);
            j--;
        }
    }
}


// Inserts the node into the tree rooted at the node pointed to by root
void insert_node(Node** root, Node* node){
    if(*root == NULL) {
        *root = node;
    } else {
        if((*root)->value > node->value) {
            insert_node(&(*root)->left, node);
        } else {
            insert_node(&(*root)->right, node);
        }
    }
}


// Searches for the number n in the tree rooted at root.
// Should return 1 if the number is present, and 0 if not.
int search(Node* root, int n){
    return 42;
}


// Returns a dynamically allocated node, with all fields set to NULL/0
Node* create_blank_node(){
    Node* blank_node = (Node*)malloc(sizeof(Node));
    blank_node->left = NULL;
    blank_node->right = NULL;
    blank_node->value = 0;
    return blank_node;
}


// Builds a tree of all the numbers in an array
Node* create_tree(int* array, int size){
    Node *root = NULL;

    for(int i = 0; i < size; i++) {
        Node *current = create_blank_node();
        current->value = array[i];
        insert_node(&root, current);
    }

    return root;
}


// Prints all the nodes of the tree.
void print_tree(Node* n, int offset){
    if(n == NULL) return;

    for(int i = 0; i < offset; i++) {
        printf(" ");
    }
    printf("%d\n", n->value);

    print_tree(n->left, offset+1);
    print_tree(n->right, offset+1);
}


// Computes x^2
double x_squared(double x){
    return x*x;
}


// Computes x^3
double x_cubed(double x){
    return x*x*x;
}


// Computes the definite integral of the function using the rectangle method
double integrate(double (*function)(double), double start, double end, double stepsize){
    return 42.0;
}


int main(int argc, char** argv){

    // Creates an array with random values
    int* array = create_random_array(10, 10);

    // Prints the values of the array, e.g:
    // 3 6 7 5 3 5 6 2 9 1
    print_array(array, 10);

    // Sorts the array
    sort(array, 10);

    // Prints the sorted array, e.g:
    // 1 2 3 3 5 5 6 6 7 9
    print_array(array, 10);

    // Create another random array
    int* new_array = create_random_array(10,10);

    // Print the second array
    print_array(new_array, 10);

    // Create a tree with the values in the new array
    Node* root = create_tree(new_array, 10);

    // Print the tree
    print_tree(root, 0);

    /*
    // Search for the values 3 and 11 in the tree
    // and print the results
    int found_3 = search(root, 3);
    int found_11 = search(root, 11);
    printf("%d, %d\n", found_3, found_11);

    // Integrate x^2 and x^3 from 0 to 1.
    // Should be approx 1/3 and 1/4.
    printf("%f\n", integrate(&x_squared, 0, 1, 0.001));
    printf("%f\n", integrate(&x_cubed, 0, 1, 0.001));
    */
}




