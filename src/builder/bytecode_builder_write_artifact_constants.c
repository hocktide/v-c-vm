#include <vcvm/builder.h>
#include <vpr/doubly_linked_list.h>
#include <string.h>
#include <vcvm/size.h>
#include <vcvm/error_codes.h>

int bytecode_builder_write_artifact_constants(bytecode_builder_t* builder, uint8_t* byte, size_t* offset)
{
    doubly_linked_list_t** buckets = (doubly_linked_list_t**)builder->artifacts.buckets;
    size_t cap = builder->artifacts.options->capacity;

    memcpy(byte + *offset, &builder->artifact_count, sizeof(uint32_t));
    *offset += sizeof(uint32_t);

    for (size_t i = 0; i < cap; i++)
    {
        doubly_linked_list_t* bucket = buckets[i];
        if (bucket == NULL)
        {
            continue;
        }

        doubly_linked_list_element_t* element = bucket->first;
        while (element != NULL)
        {
            hashmap_entry_t* entry = (hashmap_entry_t*)element->data;
            uuid_constant_t* uuid = (uuid_constant_t*) entry->val;
            memcpy(byte + *offset, uuid->value, UUID_SIZE);
            *offset += UUID_SIZE;
            element = element->next;
        }
    }

    return VCVM_STATUS_SUCCESS;
}