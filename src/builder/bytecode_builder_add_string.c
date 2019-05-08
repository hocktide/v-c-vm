#include <vpr/parameters.h>
#include <vcvm/builder.h>
#include <vpr/allocator.h>
#include <vpr/dynamic_array.h>
#define __USE_XOPEN_EXTENDED true
#include <string.h>

int bytecode_builder_add_string(bytecode_builder_t* builder, char* string)
{
    string_constant_t* constant = hashmap_get(&builder->strings, (uint8_t*)string, strlen(string));
    if (constant != NULL)
    {
        return constant->index;
    }

    string_constant_t new_constant = {
        .index = builder->string_count++,
        .value = strdup(string)
    };

    // This stops me having to iterate all the strings when working out the size of the total bytecode chunk.
    builder->string_size += strlen(string);

    hashmap_put(&builder->strings, (uint8_t*)string, strlen(string), &new_constant);

    return new_constant.index;
}