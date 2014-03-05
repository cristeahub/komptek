#include "bindnames.h"
extern int outputStage; // This variable is located in vslc.c
char* thisClass;

int b_d(node_t* root, int stack_offset);
int b_c(node_t* root, int stack_offset);

int bind_default ( node_t *root, int stackOffset)
{
	return b_d(root,stackOffset);
}


int bind_function ( node_t *root, int stackOffset)
{

	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION: Start: %s\n", root->label);

    scope_add();

    //parameter list
    node_t *parameter_list = root->children[0];
    if(parameter_list != NULL) {
        stackOffset = 8 + parameter_list->n_children * 4;
        for(int i = 0; i < parameter_list->n_children; i++) {
            if(parameter_list->children[i] != NULL) {
                node_t *child = parameter_list->children[i];
                stackOffset = child->bind_names(child, stackOffset);
            }
        }
    }

    // local variable list
    node_t *local_var_list = root->children[1];
    if(local_var_list != NULL) {
        stackOffset = -4;
        for(int i = 0; i < local_var_list->n_children; i++) {
            if(local_var_list->children[i] != NULL) {
                node_t *child = local_var_list->children[i];
                stackOffset = child->bind_names(child, stackOffset);
            }
        }
    }

    stackOffset = 0;

    scope_remove();

	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION: End\n");

    return stackOffset;

}

int bind_declaration_list ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "DECLARATION_LIST: Start\n");


	if(outputStage == 6)
		fprintf ( stderr, "DECLARATION_LIST: End\n");

    return stackOffset;

}

int bind_class ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf(stderr, "CLASS: Start: %s\n", root->children[0]->label);

	class_symbol_t *class_symbol = malloc(sizeof(class_symbol_t));
    class_symbol->symbols = ght_create(8);
    class_symbol->functions = ght_create(8);

    thisClass = root->label;

    node_t *declaration_list = root->children[0];
    if(declaration_list != NULL) {
        class_symbol->size = 4 * declaration_list->n_children;
        class_add(root->label, class_symbol);

        for(int i = 0; i < declaration_list->n_children; i++) {
            node_t *child = declaration_list->children[i];
            if(child != NULL)
                class_insert_field(root->label, child->label, create_symbol(child, stackOffset));
        }
    } else {
        class_symbol->size = 0;
        class_add(root->label, class_symbol);
    }

    node_t *function_list = root->children[1];
    if(function_list != NULL) {
        // add to symbol table
        for(int i = 0; i < function_list->n_children; i++) {
            node_t *child = function_list->children[i];
            if(child != NULL)
                class_insert_method(root->label, child->label, create_function_symbol(child));
        }

        // bind functions
        for(int i = 0; i < function_list->n_children; i++) {
            node_t *child = function_list->children[i];

            if(child != NULL) {
                stackOffset = child->bind_names(child, stackOffset);
            }
        }
    }

	if(outputStage == 6)
			fprintf(stderr, "CLASS: End\n");

    return stackOffset;

}

function_symbol_t* create_function_symbol(node_t* function_node)
{
    function_symbol_t *new_symbol = malloc(sizeof(function_symbol_t));

    new_symbol->label = STRDUP(function_node->label);
    new_symbol->return_type = function_node->data_type;

    // the first child is the node which children we want the types of
    node_t* argument_list = function_node->children[0];

    if(argument_list != NULL) {
        new_symbol->nArguments = argument_list->n_children;
        data_type_t *types_list = malloc(argument_list->n_children * sizeof(data_type_t));
        for(int i = 0; i < argument_list->n_children; i++) {
            if(argument_list->children[i] != NULL)
                types_list[i] = argument_list->children[i]->data_type;
        }
        new_symbol->argument_types = types_list;
    } else {
        new_symbol->nArguments = 0;
        new_symbol->argument_types = NULL;
    }

    return new_symbol;
}

int bind_function_list ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION_LIST: Start\n");

    // add functions to symbol table
	for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL) {
            function_add(root->children[i]->label, create_function_symbol(root->children[i]));
        }
    }

    for(int i = 0; i < root->n_children; i++) {
        if(root->children[i] != NULL) {
            root->children[i]->bind_names(root->children[i], stackOffset);
        }
    }

	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION_LIST: End\n");

    return stackOffset;

}

int bind_constant ( node_t *root, int stackOffset)
{
	return b_c(root, stackOffset);
}


symbol_t* create_symbol(node_t* declaration_node, int stackOffset)
{
    symbol_t *new_symbol = malloc(sizeof(symbol_t));

    new_symbol->stack_offset = stackOffset;
    new_symbol->depth = 0;
    new_symbol->label = STRDUP(declaration_node->label);
    new_symbol->type = declaration_node->data_type;

    return new_symbol;
}

int bind_declaration ( node_t *root, int stackOffset)
{

	if(outputStage == 6)
		fprintf(stderr, "DECLARATION: parameter/variable : '%s', offset: %d\n", root->label, stackOffset);

    symbol_insert(root->label, create_symbol(root, stackOffset));

    return stackOffset - 4;

}

int bind_variable ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "VARIABLE: access: %s\n", root->label);

    symbol_t *symbol_entry = symbol_get(root->label);
    
    root->entry = symbol_entry;

    return stackOffset - 4;

}

int bind_expression( node_t* root, int stackOffset)
{
	if(outputStage == 6)
		fprintf( stderr, "EXPRESSION: Start: %s\n", root->expression_type.text);

    switch(root->expression_type.index) {
        case NEW_E:
            {
                node_t *class_name = root->children[0];
                root->class_entry = class_get(class_name->data_type.class_name);
            }
            break;

        case THIS_E:
            {
                symbol_t *symbol = malloc(sizeof(symbol_t));
                data_type_t type = (data_type_t) { .class_name = thisClass, .base_type = CLASS_TYPE };

                symbol->type = type;
                symbol->stack_offset = 8;
                symbol->depth = 0;

                root->entry = symbol;
            }
            break;

        case CLASS_FIELD_E:
            {
                stackOffset = 0;
                node_t *class_name = root->children[0];
                stackOffset = class_name->bind_names(class_name, stackOffset);

                node_t *field_name = root->children[1];
                symbol_t *symbol = class_get_symbol(class_name->entry->type.class_name, field_name->label);

                field_name->entry = symbol;
                root->entry = symbol;

            }
            break;

        case METH_CALL_E:
            {
                node_t *class_name = root->children[0];
                stackOffset = class_name->bind_names(class_name, stackOffset);

                node_t *field_name = root->children[1];
                symbol_t *symbol = class_get_method(class_name->entry->type.class_name, field_name->label);

                root->function_entry = symbol;

                node_t *argument_list = root->children[2];
                if (argument_list != NULL) {
                    for(int i = 0; i < argument_list->n_children; i++) {
                        if(argument_list->children[i] != NULL)
                            argument_list->children[i]->bind_names(argument_list->children[i], stackOffset);
                    }
                }
            }
            break;

        case FUNC_CALL_E:
            {
                node_t *function_node = root->children[0];
                node_t *argument_list = root->children[1];

                function_symbol_t *function_symbol = function_get(function_node->label);
                root->function_entry = function_symbol;

                if (argument_list != NULL) {
                    for (int i=0; i < argument_list->n_children; i++) {
                        if (argument_list->children[i] != NULL) {
                            argument_list->children[i]->bind_names(argument_list->children[i], stackOffset);
                        }
                    }
                }
            }
            break;

        default:
            {
                for(int i = 0; i < root->n_children; i++) {
                    if(root->children[i] != NULL)
                        root->children[i]->bind_names(root->children[i], stackOffset);
                }
            }
            break;
    }


	if(outputStage == 6)
		fprintf( stderr, "EXPRESSION: End\n");

    return stackOffset;

}


