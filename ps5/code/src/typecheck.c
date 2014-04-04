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
    return (a.base_type == b.base_type);
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
	
    et_number expression_type_number = root->expression_type.index;
    switch(expression_type_number) {
        case FUNC_CALL_E:
        case METH_CALL_E:
            ;
            int list_index = (expression_type_number == FUNC_CALL_E) ? 1 : 2;
            function_symbol_t *func_sym = root->function_entry;
            node_t *expression_list = root->children[list_index];

            if(expression_list == NULL) {
                if(func_sym->nArguments != 0) {
                    type_error(root);
               }
            } else {
                if(func_sym->nArguments == expression_list->n_children) {
                    for(int i=0; i < expression_list->n_children; i++) {
                        node_t *child = expression_list->children[i];
                        if(!equal_types(child->data_type, func_sym->argument_types[i])) {
                            type_error(root);
                        }
                    }
                } else {
                    type_error(root);
                }
            }
            toReturn = root->function_entry->return_type;
            break;

    case CLASS_FIELD_E:
            toReturn = root->entry->type;
            break;
    }
    root->data_type = toReturn;

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

    node_t *expression = root->children[1];

    data_type_t type_left = root->children[0]->entry->type;
    data_type_t type_right = expression->typecheck(expression);

    if(!equal_types(type_left, type_right)) {
        type_error(root);
    }

    return type_left;
}
