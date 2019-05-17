#include <vcvm/instruction.h>
#include <vcvm/error_codes.h>
#include <vcvm/vm.h>
#include <vcvm/stack_value.h>
#include <vpr/parameters.h>

int and_(vm_t* vm)
{
    int result;
    stack_value_t* left;
    stack_value_t* right;

    result = vm_pop(vm, &right);
    if (result != VCVM_STATUS_SUCCESS)
    {
        return result;
    }

    result = vm_pop(vm, &left);
    if (result != VCVM_STATUS_SUCCESS)
    {
        goto cleanup_right;
    }

    if (left->type != STACK_VALUE_TYPE_INTEGER || right->type != STACK_VALUE_TYPE_INTEGER)
    {
        result = VCVM_ERROR_VM_BAD_TYPES;
        goto cleanup_both;
    }

    stack_value_t* value = (stack_value_t*)allocate(vm->allocator_options, sizeof(stack_value_t));
    if (value == NULL)
    {
        result = VCVM_ERROR_CANT_ALLOCATE;
        goto cleanup_both;
    }

    stack_value_init(value, vm->allocator_options);
    stack_value_set_int(value, left->integer & right->integer);

    result = vm_push(vm, value);

cleanup_both:
    dispose((disposable_t*)left);
    release(left->allocator_options, left);

cleanup_right:
    dispose((disposable_t*)right);
    release(right->allocator_options, right);

    return result;
}

const instruction_t AND_ = {
    .name = "AND_",
    .arity = 0,
    .handler = {
        .arity0 = &and_,
    }
};
