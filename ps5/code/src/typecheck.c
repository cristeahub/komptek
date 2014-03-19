#include "typecheck.h"
extern int outputStage;

data_type_t td(node_t* root);
data_type_t te(node_t* root);
data_type_t tv(node_t* root);

void type_error(node_t* root){
	fprintf(stderr, "Type error at:\n");
	if(root != NULL){
		fprintf(stderr,"%s", root->nodetype.text);
		if(root->nodetype.index == EXPRESSION){
			fprintf(stderr," (%s)", root->expression_type.text);
		}
		fprintf(stderr,"\n");
	}
	fprintf(stderr,"Exiting\n");
	exit(-1);
}

int equal_types(data_type_t a, data_type_t b)
{
    return (a.base_type == b.base_type) ? 1 : 0;
}

data_type_t typecheck_default(node_t* root)
{
	return td(root);
}

data_type_t typecheck_expression(node_t* root)
{
	data_type_t toReturn;

	if(outputStage == 10)
		fprintf( stderr, "Type checking expression %s\n", root->expression_type.text);

	toReturn = te(root);
	
    // Check if argument list has the same length as children
    if(root->n_children != root->function_entry->nArguments) {
        type_error(root);
    }

    for(int i = 0; i < root->n_children; i++) {
        node_t *child = root->children[i];
        if(child != NULL) {
            if(!equal_types(child->data_type, root->function_entry->argument_types[i])) {
                type_error(root);
            }
        }
    }
    

	return toReturn;
}

data_type_t typecheck_variable(node_t* root){
	return tv(root);
}

data_type_t typecheck_assignment(node_t* root)
{
    data_type_t to_return;

	if(outputStage == 10)
		fprintf( stderr, "Type checking expression %s\n", root->expression_type.text);

	to_return = te(root);

    data_type_t first = root->children[0]->typecheck(root->children[0]);
    data_type_t second = root->children[1]->typecheck(root->children[1]);

    if(!equal_types(first, second)) {
        type_error(root);
    }

    return to_return;
}
