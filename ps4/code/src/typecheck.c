#include "typecheck.h"
extern int outputStage;

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



data_type_t typecheck_default(node_t* root)
{
    for (int i = 0; i < root->n_children; i++) {
        if (root->children[i] != NULL) {
            root->children[i]->typecheck(root->children[i]);
        }
    }
    return root->data_type;
}

data_type_t typecheck_expression(node_t* root)
{

	if(outputStage == 10)
		fprintf( stderr, "Type checking expression %s\n", root->expression_type.text);

    switch (root->expression_type.index) {
        case ADD_E:
        case SUB_E:
        case DIV_E:
        case MUL_E:
            {
                node_t *child = root->children[0];
                data_type_t child_type = child->typecheck(child);
                node_t *right = root->children[0];
                data_type_t right_type = right->typecheck(right);

                if (child_type.base_type == right_type.base_type) {
                    if (child_type.base_type == INT_TYPE || child_type.base_type == FLOAT_TYPE) {
                        return child_type;
                    }
                }
                type_error(root);
            }
            break;
        case LEQUAL_E:
        case GEQUAL_E:
        case LESS_E:
        case GREATER_E:
            {
                node_t *child = root->children[0];
                data_type_t child_type = child->typecheck(child);
                node_t *right = root->children[0];
                data_type_t right_type = right->typecheck(right);

                if (child_type.base_type == right_type.base_type) {
                    if (child_type.base_type == INT_TYPE || child_type.base_type == FLOAT_TYPE) {
                        return (data_type_t){.base_type = BOOL_TYPE};
                    }
                }
                type_error(root);
            }
            break;
        case EQUAL_E:
        case NEQUAL_E:
            {
                node_t *child = root->children[0];
                data_type_t child_type = child->typecheck(child);
                node_t *right = root->children[0];
                data_type_t right_type = child->typecheck(right);

                if (child_type.base_type == right_type.base_type) {
                    if (child_type.base_type == INT_TYPE || child_type.base_type == FLOAT_TYPE || child_type.base_type == BOOL_TYPE) {
                        return (data_type_t){.base_type = BOOL_TYPE};
                    }
                }
                type_error(root);
            }
            break;
        case UMINUS_E:
            {
                node_t *child = root->children[0];
                data_type_t child_type = child->typecheck(child);
                if (child_type.base_type == INT_TYPE || child_type.base_type == FLOAT_TYPE) {
                    return child_type;
                }
                type_error(root);
            }
            break;
        case NOT_E:
            {
                node_t *child = root->children[0];
                data_type_t child_type = child->typecheck(child);
                if (child_type.base_type == BOOL_TYPE) {
                    return child_type;
                }
                type_error(root);
            }
            break;
        case AND_E:
        case OR_E:
            {
                node_t *child = root->children[0];
                data_type_t child_type = child->typecheck(child);
                node_t *right = root->children[0];
                data_type_t right_type = child->typecheck(right);

                if (child_type.base_type == right_type.base_type) {
                    if (child_type.base_type == BOOL_TYPE) {
                        return (data_type_t){.base_type = BOOL_TYPE};
                    }
                }
                type_error(root);
            }
            break;
        default:
            {
                return typecheck_default(root);
            }
            break;
    }
}

data_type_t typecheck_variable(node_t* root)
{
    if (root->entry != NULL) {
        return root->entry->type;
    }
    return (data_type_t) {.base_type = NO_TYPE};
}
