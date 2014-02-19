#include "simplifynodes.h"

extern int outputStage; // This variable is located in vslc.c

Node_t* simplify_default ( Node_t *root, int depth )
{
	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    return root;
}


Node_t *simplify_types ( Node_t *root, int depth )
{
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    if (root->data_type.base_type == CLASS_TYPE) {
        root->data_type.class_name = STRDUP(root->children[0]->label); // first child is class name

        free(root->children[0]);
        root->n_children = 0;
    }
    return root;

}


Node_t *simplify_function ( Node_t *root, int depth )
{
    // root->children[0] is data_type
    // root->children[1] is label
    // root->children[0] is parameter_list
    // root->children[0] is statement_list

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    root->data_type = root->children[0]->data_type;
    root->label = STRDUP(root->children[1]->label);

    free(root->children[0]);
    free(root->children[1]);

    root->children[0] = root->children[2];
    root->children[1] = root->children[3];

    root->children = realloc(root->children, 2*sizeof(Node_t*));
    root->n_children = 2;

    return root;

}


Node_t *simplify_class( Node_t *root, int depth )
{
    // root->children[0] is label
    // root->children[0] is dec_list
    // root->children[0] is func_list

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    root->label = STRDUP(root->children[0]->label);

    int new_array_size = 2;
    Node_t **new_array = malloc(new_array_size * sizeof(Node_t*));

    for(int i = 0; i < root->n_children - 1; i++) {
        new_array[i] = root->children[i+1];
    }

    free(root->children);

    root->children = new_array;
    root->n_children = new_array_size;

    return root;
}


Node_t *simplify_declaration_statement ( Node_t *root, int depth )
{
    // root->children[0] is data_type
    // root->children[0] is label

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    root->data_type = root->children[0]->data_type;
    root->label = STRDUP(root->children[1]->label);

    for(int i = 0; i < root->n_children; i++) {
        free(root->children[i]);
    }
    free(root->children);
    root->n_children = 0;

    return root;
}


Node_t *simplify_single_child ( Node_t *root, int depth )
{
    // root->children[0] is variable
    // root->children[1] is a list
    // first element of that list is what we might want to simplify

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );
	

	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    if(root->n_children == 1) {
        return root->children[0];
    }

    return root;
}

Node_t *simplify_list_with_null ( Node_t *root, int depth )
{
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    if(root->children[0] == NULL) {
        root->children[0] = root->children[1];
        root->n_children = 1;
    } else {
        int new_array_size = root->children[0]->n_children + 1;
        Node_t **new_array = malloc(new_array_size * sizeof(Node_t*));

        // copy children over to new array
        for(int i = 0; i <root->children[0]->n_children; i++) {
            new_array[i] = root->children[0]->children[i];
        }
        new_array[new_array_size-1] = root->children[1];

        free(root->children);

        root->children = new_array;
        root->n_children = new_array_size;
    }
    return root;
}


Node_t *simplify_list ( Node_t *root, int depth )
{
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    if(root->n_children == 1) return root;

    int new_array_size = root->children[0]->n_children + 1;
    Node_t **new_array = malloc(new_array_size * sizeof(Node_t*));

    // copy children over to new array
    for(int i = 0; i <root->children[0]->n_children; i++) {
        new_array[i] = root->children[0]->children[i];
    }
    new_array[new_array_size-1] = root->children[1];

    free(root->children);
    root->children = new_array;
    root->n_children = new_array_size;

    return root;
}


Node_t *simplify_expression ( Node_t *root, int depth )
{
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s (%s) \n", depth, ' ', root->nodetype.text, root->expression_type.text );

	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL)
            root->children[i] = root->children[i]->simplify(root->children[i], depth+1);
    }

    if(root->n_children == 1 &&
            !(root->nodetype.index == NEW_E || root->nodetype.index == UMINUS_E
                || root->nodetype.index == NOT_E)) {
        return root->children[0];
    }

    return root;

}
