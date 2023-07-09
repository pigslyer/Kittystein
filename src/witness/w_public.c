#include "w_internal.h"


void witness_commands_execute(uint argc, char** argv)
{
    for (uint i = 0; i < functionCount; i++)
    {
        if (delight_string_equals(argv[0], functions[i].functionName))
        {
            functions[i].function(argc, argv);
        }
    }
}