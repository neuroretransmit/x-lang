#include <stdio.h>

#include <dumps.h>
#include <arguments.h>
#include <util/file_utils.h>

int main(int argc, char** argv)
{
	struct arguments arguments = parse_arguments(argc, argv);
    
    if (file_exists(arguments.args[0])) {
        if (arguments.ast)
            dump_ast(arguments.args[0]);
        
        if (arguments.ir) {
            if (arguments.ast)
                puts(DIVIDER);
            dump_ir(arguments.args[0]);
        }
        
        if (arguments.bitcode)
            dump_bitcode(arguments.args[0], arguments.bitcode);
        
        if (arguments._asm)
            dump_asm(arguments.args[0], arguments._asm);
        
        if (arguments.outfile)
            dump_binary(arguments.args[0], arguments.outfile);
    }
}	
