#include "w_internal.h"


void witness_commands_execute(u32 argc, char** argv)
{
    for (u32 i = 0; i < functionCount; i++)
    {
        if (string_equals(argv[0], functions[i].functionName))
        {
            functions[i].function(argc, argv);
        }
    }
}